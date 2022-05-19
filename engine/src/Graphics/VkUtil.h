#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

namespace Wraith
{
    template <typename T>
    T HandleResult(vk::ResultValue<T>&& result_value, std::initializer_list<vk::Result> successful_results = { vk::Result::eSuccess })
    {
        bool succeeded = false;
        for (auto& s_result : successful_results)
        {
            if (result_value.result == s_result)
            {
                succeeded = true;
            }
        }

        if (!succeeded)
        {
            ASSERT_LOG("Result was unsuccessful!");
        }

        return std::forward<T>(result_value.value);
    }
}  // namespace Wraith