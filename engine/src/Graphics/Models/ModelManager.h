#pragma once

#include <unordered_map>

#include "Core/Singleton.h"
#include "Model.h"

namespace Wraith
{
    class ModelManager : public Singleton<ModelManager>
    {
    public:
        // static Model& GetCube();

    private:
        // std::unordered_map<StringID, Model> m_ModelMap;
    };
}  // namespace Wraith