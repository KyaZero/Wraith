#pragma once

#include "Core/Math/Vec4.h"
#include "Texture.h"
#include "Window/Window.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Wraith
{
    class Framework
    {
    public:
        Framework(Window& window);
        ~Framework();

        bool Init();

        void BeginFrame(const Vec4f& clear_color);
        void EndFrame();

        void SetBackbufferAsActiveTarget();

        static ID3D11Device* GetDevice();
        static ID3D11DeviceContext* GetContext();
        static void ReportLiveObjects();

        static void BeginEvent(std::string name);
        static void EndEvent();

    private:
        void ProcessDebugMessages();
        void CreateBackbufferRTV();
        void ResizeBackbuffer(u32 width, u32 height);

        Window& m_Window;
        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}  // namespace Wraith