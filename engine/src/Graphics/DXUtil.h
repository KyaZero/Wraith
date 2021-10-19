#pragma once

#include "Core/Logger.h"

namespace Wraith
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

    inline void SetDebugObjectName([[maybe_unused]] ID3D11DeviceChild* resource, [[maybe_unused]] std::string_view name)
    {
#ifdef _DEBUG
        static int id = 0;
        std::string n = std::format("#Wraith::{}[{}]", name, ++id);
        resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<u32>(n.size()), n.data());
#endif
    }
};  // namespace Wraith