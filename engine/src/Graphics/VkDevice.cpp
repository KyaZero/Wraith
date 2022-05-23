#include "VkDevice.h"

#include <set>
#include <unordered_set>

#include "Core/Logger.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                    VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                    void* user_data)
{
    if (message_severity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        ERROR_LOG("Validation Layer: {}", callback_data->pMessage);
        return VK_FALSE;
    }
    else if (message_severity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        WARNING_LOG("Validation Layer: {}", callback_data->pMessage);
        return VK_FALSE;
    }
    else if (message_severity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        INFO_LOG("Validation Layer: {}", callback_data->pMessage);
        return VK_FALSE;
    }
    else if (message_severity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        VERBOSE_LOG("Validation Layer: {}", callback_data->pMessage);
        return VK_FALSE;
    }

    return VK_TRUE;
}

Wraith::Device::Device(Window& window)
    : m_Window(window)
{
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateCommandPool();
}

Wraith::Device::~Device()
{
    DestroyDebugMessenger();
}

u32 Wraith::Device::FindMemoryType(u32 type_filter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties mem_props = m_PhysicalDevice.getMemoryProperties();
    for (u32 i = 0; i < mem_props.memoryTypeCount; ++i)
    {
        if ((type_filter & (1 << i)) && (mem_props.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    ASSERT_LOG(false, "Failed to find suitable memory type!");
    return {};
}

vk::Format Wraith::Device::FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
{
    for (vk::Format format : candidates)
    {
        vk::FormatProperties props = m_PhysicalDevice.getFormatProperties(format);

        if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    ASSERT_LOG(false, "Failed to find supported format!");
    return {};
}

void Wraith::Device::CreateImageWithInfo(const vk::ImageCreateInfo& image_info,
                                         vk::MemoryPropertyFlags properties,
                                         vk::UniqueImage& image,
                                         vk::UniqueDeviceMemory& image_memory)
{
    image = HandleResult(m_Device->createImageUnique(image_info));

    vk::MemoryRequirements requirements = m_Device->getImageMemoryRequirements(*image);
    vk::MemoryAllocateInfo alloc_info(requirements.size, FindMemoryType(requirements.memoryTypeBits, properties));

    image_memory = HandleResult(m_Device->allocateMemoryUnique(alloc_info));

    if (m_Device->bindImageMemory(*image, *image_memory, 0) != vk::Result::eSuccess)
    {
        ASSERT_LOG(false, "Failed to bind image memory!");
    }
}

void Wraith::Device::CreateInstance()
{
    if (GetValidationLayersEnabled() && !CheckValidationLayerSupport())
    {
        ASSERT_LOG(false, "Validation layers requested, but not available!");
    }

    vk::ApplicationInfo app_info("Wraith", VK_MAKE_VERSION(1, 0, 0), "Wraith Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_2);

    auto required_extensions = GetRequiredExtensions();

    vk::InstanceCreateInfo instance_info({}, &app_info, 0, nullptr, (u32)required_extensions.size(), required_extensions.data());

    auto validation_layers = GetValidationLayers();
    if (GetValidationLayersEnabled())
    {
        instance_info.enabledLayerCount = (u32)validation_layers.size();
        instance_info.ppEnabledLayerNames = validation_layers.data();
    }

    m_Instance = HandleResult(vk::createInstanceUnique(instance_info));

    HasGLFWRequiredInstanceExtensions();
}

void Wraith::Device::SetupDebugMessenger()
{
    if (!GetValidationLayersEnabled())
        return;

    auto severity_flags = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                          vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;

    auto type_flags =
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

    vk::DispatchLoaderDynamic dispatch_loader(*m_Instance, vkGetInstanceProcAddr);
    vk::DebugUtilsMessengerCreateInfoEXT messenger_create_info({}, severity_flags, type_flags, DebugCallback);
    m_DebugMessenger = HandleResult(m_Instance->createDebugUtilsMessengerEXT(messenger_create_info, nullptr, dispatch_loader));
}

void Wraith::Device::DestroyDebugMessenger()
{
    if (!GetValidationLayersEnabled())
        return;
    vk::DispatchLoaderDynamic dispatch_loader(*m_Instance, vkGetInstanceProcAddr);
    m_Instance->destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, dispatch_loader);
}

void Wraith::Device::CreateSurface()
{
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(*m_Instance, m_Window.GetHandle(), nullptr, &surface) != VK_SUCCESS)
    {
        ASSERT_LOG(false, "Failed to create window surface!");
        return;
    }

    m_Surface = vk::UniqueSurfaceKHR(surface, *m_Instance);
}

void Wraith::Device::PickPhysicalDevice()
{
    std::vector<vk::PhysicalDevice> physical_devices = HandleResult(m_Instance->enumeratePhysicalDevices());

    VERBOSE_LOG("Available Physical Devices: ");
    for (auto& device : physical_devices)
    {
        VERBOSE_LOG("\t {}", device.getProperties().deviceName);
    }

    for (const auto& device : physical_devices)
    {
        if (IsPreferredDevice(device))
        {
            m_PhysicalDevice = device;
            break;
        }
    }

    for (const auto& device : physical_devices)
    {
        if (IsSuitableDevice(device))
        {
            m_PhysicalDevice = device;
            break;
        }
    }

    if (!m_PhysicalDevice)
    {
        ASSERT_LOG(false, "Failed to find a suitable GPU!");
        return;
    }

    m_Properties = m_PhysicalDevice.getProperties();
    INFO_LOG("Selected GPU: {}", m_Properties.deviceName);
}

void Wraith::Device::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
    std::set<u32> unique_queue_families = { indices.graphics_family, indices.present_family };

    f32 queue_priority = 1.0f;
    for (u32 queue_family : unique_queue_families)
    {
        vk::DeviceQueueCreateInfo create_info({}, queue_family, queue_priority);
        create_info.queueCount = 1;
        create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(create_info);
    }

    vk::PhysicalDeviceFeatures device_features = {};
    device_features.samplerAnisotropy = VK_TRUE;
    device_features.fillModeNonSolid = VK_TRUE;

    auto device_extensions = GetDeviceExtensions();
    vk::DeviceCreateInfo create_info({}, queue_create_infos, {}, device_extensions, &device_features);

    m_Device = HandleResult(m_PhysicalDevice.createDeviceUnique(create_info));

    m_GraphicsQueue = m_Device->getQueue(indices.graphics_family, 0);
    m_PresentQueue = m_Device->getQueue(indices.present_family, 0);
}

void Wraith::Device::CreateCommandPool()
{
    QueueFamilyIndices queue_family_indices = FindPhysicalQueueFamilies();

    vk::CommandPoolCreateInfo pool_info(vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                        queue_family_indices.graphics_family);

    m_CommandPool = HandleResult(m_Device->createCommandPoolUnique(pool_info));
}

std::vector<const char*> Wraith::Device::GetValidationLayers()
{
    const std::vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation" };
    return validation_layers;
}

std::vector<const char*> Wraith::Device::GetRequiredExtensions()
{
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> required_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if (GetValidationLayersEnabled())
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return required_extensions;
}

std::vector<const char*> Wraith::Device::GetDeviceExtensions()
{
    const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    return device_extensions;
}

bool Wraith::Device::IsSuitableDevice(vk::PhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamilies(device);

    bool extensions_supported = CheckDeviceExtensionSupport(device);

    bool swap_chain_adequate = false;
    if (extensions_supported)
    {
        SwapChainSupportDetails swap_chain_support = QuerySwapChainSupport(device);
        swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
    }

    vk::PhysicalDeviceFeatures supported_features = device.getFeatures();

    bool surface_supported = HandleResult(device.getSurfaceSupportKHR(indices.graphics_family, *m_Surface));

    return indices.IsComplete() && extensions_supported && swap_chain_adequate && supported_features.samplerAnisotropy && surface_supported;
}

bool Wraith::Device::IsPreferredDevice(vk::PhysicalDevice device)
{
    if (!IsSuitableDevice(device))
    {
        return false;
    }

    vk::PhysicalDeviceProperties properties = device.getProperties();

    return properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
}

Wraith::Device::QueueFamilyIndices Wraith::Device::FindQueueFamilies(vk::PhysicalDevice device)
{
    QueueFamilyIndices indices;
    std::vector<vk::QueueFamilyProperties> queue_families = device.getQueueFamilyProperties();

    i32 i = 0;
    for (const auto& family : queue_families)
    {
        if (family.queueCount > 0 && family.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.graphics_family = i;
            indices.has_graphics = true;
        }

        vk::Bool32 present_support = HandleResult(device.getSurfaceSupportKHR(i, *m_Surface));
        if (family.queueCount > 0 && present_support)
        {
            indices.present_family = i;
            indices.has_present = true;
        }

        if (indices.IsComplete())
            break;

        ++i;
    }

    return indices;
}

void Wraith::Device::HasGLFWRequiredInstanceExtensions()
{
    std::vector<vk::ExtensionProperties> extensions = HandleResult(vk::enumerateInstanceExtensionProperties());

    VERBOSE_LOG("Available Vulkan Instance Extensions: ");

    std::unordered_set<std::string> available;
    for (const auto& extension : extensions)
    {
        VERBOSE_LOG("\t {}", extension.extensionName);
        available.insert(extension.extensionName);
    }

    VERBOSE_LOG("Required Extensions: ");
    auto required_extensions = GetRequiredExtensions();
    for (const auto& required : required_extensions)
    {
        VERBOSE_LOG("\t {}", required);
        if (available.find(required) == available.end())
        {
            VERBOSE_LOG("Missing required glfw extension");
        }
    }
}

bool Wraith::Device::CheckDeviceExtensionSupport(vk::PhysicalDevice device)
{
    std::vector<vk::ExtensionProperties> available_extensions = HandleResult(device.enumerateDeviceExtensionProperties());

    auto device_extensions = GetDeviceExtensions();
    std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

    for (const auto& extension : available_extensions)
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

Wraith::Device::SwapChainSupportDetails Wraith::Device::QuerySwapChainSupport(vk::PhysicalDevice device)
{
    SwapChainSupportDetails details;
    details.capabilities = HandleResult(device.getSurfaceCapabilitiesKHR(*m_Surface));
    details.formats = HandleResult(device.getSurfaceFormatsKHR(*m_Surface));
    details.present_modes = HandleResult(device.getSurfacePresentModesKHR(*m_Surface));
    return details;
}

bool Wraith::Device::CheckValidationLayerSupport()
{
    std::vector<vk::LayerProperties> available_layers = HandleResult(vk::enumerateInstanceLayerProperties());

    for (const char* layer_name : GetValidationLayers())
    {
        bool layer_found = false;
        for (const auto& layer_props : available_layers)
        {
            if (strcmp(layer_name, layer_props.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }

        if (!layer_found)
            return false;
    }

    return true;
}