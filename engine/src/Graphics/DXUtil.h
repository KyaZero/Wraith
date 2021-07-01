#pragma once

#include "Core/Logger.h"

namespace fw
{
    // Returns true if failed, and false otherwise.
    inline bool FailedCheck(const std::string& name, HRESULT res)
    {
        if (FAILED(res))
        {
            ERROR_LOG("'{}' failed.", name.c_str());
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
};  // namespace fw