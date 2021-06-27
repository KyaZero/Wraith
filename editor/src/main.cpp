#include <Application.h>
#include <Core/EntryPoint.h>

#include "Editor.h"

namespace fw
{
    Application* CreateApplication()
    {
        return new Editor();
    }
}  // namespace fw