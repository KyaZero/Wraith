#include "VkFramework.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VULKAN_HPP_NO_EXCEPTIONS
#include <set>

#include <vulkan/vulkan.hpp>

//#include "DXUtil.h"
//#include "Texture.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

namespace Wraith
{
    template <typename T>
    T HandleResult(vk::ResultValue<T>&& resultValue, std::initializer_list<vk::Result> successfulResults = { vk::Result::eSuccess })
    {
        bool succeeded = false;
        for (auto& sResult : successfulResults)
        {
            if (resultValue.result == sResult)
            {
                succeeded = true;
            }
        }

        if (!succeeded)
        {
            ASSERT_LOG("Result was unsuccessful!");
        }

        return std::forward<T>(resultValue.value);
    }

    struct VkFramework::Data
    {
        vk::UniqueInstance instance;
        vk::UniqueSurfaceKHR surface;
        vk::PhysicalDevice physicalDevice;
        vk::UniqueDevice device;
    };

    VkFramework::VkFramework(Window& window)
        : m_Window(window)
    {
        m_Data = std::make_unique<Data>();
    }

    VkFramework::~VkFramework() { Window::UnregisterResizeCallback(this); }

    bool VkFramework::Init()
    {
        vk::ApplicationInfo appInfo("Wraith", VK_MAKE_VERSION(1, 0, 0), "Wraith Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_2);

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        requiredExtensions.push_back("VK_EXT_debug_utils");

        std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };

        vk::InstanceCreateInfo instanceInfo({}, &appInfo, 0, nullptr, (u32)requiredExtensions.size(), requiredExtensions.data());
        m_Data->instance = HandleResult(vk::createInstanceUnique(instanceInfo));

        vk::DispatchLoaderDynamic dispatchLoader(*m_Data->instance, vkGetInstanceProcAddr);

        auto severityFlags = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                             vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;

        auto typeFlags = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                         vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

        vk::DebugUtilsMessengerCreateInfoEXT messengerCreateInfo({}, severityFlags, typeFlags, debugCallback);
        auto messenger = HandleResult(m_Data->instance->createDebugUtilsMessengerEXTUnique(messengerCreateInfo, nullptr, dispatchLoader));

        INFO_LOG("Available Vulkan Instance Extensions: ")
        for (const auto& extension : HandleResult(vk::enumerateInstanceExtensionProperties()))
        {
            INFO_LOG("\t {}", extension.extensionName);
        }

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(*m_Data->instance, m_Window.GetHandle(), nullptr, &surface) != VK_SUCCESS)
        {
            return false;
        }

        m_Data->surface = vk::UniqueSurfaceKHR(surface, *m_Data->instance);

        std::vector<vk::PhysicalDevice> physicalDevices = HandleResult(m_Data->instance->enumeratePhysicalDevices());

        INFO_LOG("Available Physical Devices: ");
        for (auto& device : physicalDevices)
        {
            INFO_LOG("\t {}", device.getProperties().deviceName);
        }

        // Goes for first discrete gpu if possible, we might update this to do a more thorough selection later.
        m_Data->physicalDevice = physicalDevices[std::distance(
            physicalDevices.begin(), std::find_if(physicalDevices.begin(), physicalDevices.end(), [](const vk::PhysicalDevice& physicalDevice) {
                return physicalDevice.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
            }))];

        // Only looks for a graphics queue right now, later on we will want a compute queue, and maybe a transfer queue as well.
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_Data->physicalDevice.getQueueFamilyProperties();
        u64 graphicsQueueFamilyIndex = std::distance(
            queueFamilyProperties.begin(), std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(), [](vk::QueueFamilyProperties const& qfp) {
                return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
            }));

        u64 presentQueueFamilyIndex = 0u;
        for (auto i = 0ul; i < queueFamilyProperties.size(); i++)
        {
            if (HandleResult(m_Data->physicalDevice.getSurfaceSupportKHR((u32)i, *m_Data->surface)))
            {
                presentQueueFamilyIndex = i;
            }
        }

        std::set<u32> uniqueQueueFamilyIndices = { (u32)graphicsQueueFamilyIndex, (u32)presentQueueFamilyIndex };

        std::vector<uint32_t> FamilyIndices = { uniqueQueueFamilyIndices.begin(), uniqueQueueFamilyIndices.end() };

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

        f32 queuePriority = 0.0f;
        for (auto& queueFamilyIndex : uniqueQueueFamilyIndices)
        {
            queueCreateInfos.push_back(vk::DeviceQueueCreateInfo({}, (u32)queueFamilyIndex, 1, &queuePriority));
        }

        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        vk::DeviceCreateInfo deviceInfo(
            {}, (u32)queueCreateInfos.size(), queueCreateInfos.data(), 0u, nullptr, (u32)deviceExtensions.size(), deviceExtensions.data());

        m_Data->device = HandleResult(m_Data->physicalDevice.createDeviceUnique(deviceInfo));

        u32 imageCount = 2;

        Window::RegisterResizeCallback(this, [&](u32 width, u32 height) { ResizeBackbuffer(width, height); });
        INFO_LOG("Finished initializing Vulkan Framework!");
        return true;
    }

    void VkFramework::BeginFrame(const Vec4f& clear_color) { }

    void VkFramework::EndFrame() { }

    void VkFramework::ResizeBackbuffer(u32 width, u32 height)
    {
        if (width == 0 || height == 0)
            return;

        VERBOSE_LOG("Resized backbuffer to ({}, {})", width, height);
    }

    void VkFramework::BeginEvent(std::string name) { }

    void VkFramework::EndEvent() { }
}  // namespace Wraith