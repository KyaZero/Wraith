#pragma once
#include <d3d11.h>

#include "Core/Logger.h"

// This file should only be included in .cpp files so as to not leak d3d11.h into the headers
namespace fw
{
    // Returns true if failed, and false otherwise.
    inline bool FailedCheck(const std::string& name, HRESULT res)
    {
        if (FAILED(res))
        {
            ERROR_LOG("'%s' failed.", name.c_str());
            return true;
        }
        return false;
    }

    inline bool FailedCheck(HRESULT res)
    {
        if (FAILED(res))
        {
            return true;
        }
        return false;
    }

    template <typename T>
    inline void SafeRelease(T** resource)
    {
        (*resource)->Release();
        *resource = nullptr;
    }
};  // namespace fw