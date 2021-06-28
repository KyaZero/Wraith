#include "Framework.h"

#include <codecvt>
#include <locale>
#include <string>

#include <atlbase.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <d3d11.h>
#include <d3d11_1.h>

#include "DXUtil.h"
#include "Texture.h"

namespace fw
{
    // for getters, should figure out a better way to do this
    static ID3D11Device* s_Device;
    static ID3D11DeviceContext* s_Context;
    void* s_Annot = nullptr;

    struct Framework::Data
    {
        IDXGISwapChain* swap_chain = nullptr;
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* context = nullptr;
        Texture back_buffer;
        std::vector<IDXGIAdapter*> adapters;
    };

    Framework::Framework(Window& window)
        : m_Window(window)
    {
        m_Data = new Data;
    }

    Framework::~Framework()
    {
        Window::UnregisterResizeCallback(this);

        if (m_Data)
        {
            for (auto* adapter : m_Data->adapters)
            {
                adapter->Release();
            }

            SafeRelease(&m_Data->swap_chain);
            m_Data->back_buffer.Release();

            m_Data->context->ClearState();
            m_Data->context->Flush();

            SafeRelease(&m_Data->context);
            SafeRelease(&m_Data->device);

            delete m_Data;
        }
        m_Data = nullptr;
    }

    std::vector<IDXGIAdapter*> EnumerateAdapters()
    {
        IDXGIAdapter* adapter;
        std::vector<IDXGIAdapter*> adapters;
        IDXGIFactory* factory = nullptr;

        if (FailedCheck("Gathering adapters", CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
        {
            return adapters;
        }

        for (u32 i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            adapters.push_back(adapter);
        }

        if (factory)
        {
            factory->Release();
        }

        return adapters;
    }

    bool Framework::Init()
    {
        m_Data->adapters = EnumerateAdapters();

        IDXGIAdapter* adapter = nullptr;
        for (auto* a : m_Data->adapters)
        {
            DXGI_ADAPTER_DESC adapterDescription;
            a->GetDesc(&adapterDescription);

            INFO_LOG("Found adapter: %ls VRAM: %umb",
                     adapterDescription.Description,
                     adapterDescription.DedicatedVideoMemory / (1024 * 1024));

            if (adapter)
            {
                DXGI_ADAPTER_DESC retDescription;
                adapter->GetDesc(&retDescription);

                if (retDescription.DedicatedVideoMemory < adapterDescription.DedicatedVideoMemory)
                {
                    adapter = a;
                }
            }
            else
            {
                adapter = a;
            }
        }

        if (!adapter)
        {
            ERROR_LOG("Failed to get a graphics adapter!");
            return false;
        }

        UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
        creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
                                               D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3,  D3D_FEATURE_LEVEL_9_1 };

        DXGI_SWAP_CHAIN_DESC swapchain_desc = { 0 };
        swapchain_desc.BufferCount = 1;
        swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        swapchain_desc.OutputWindow = (HWND)m_Window.GetPlatformHandle();
        swapchain_desc.SampleDesc.Count = 1;
        swapchain_desc.Windowed = true;

        if (FailedCheck("D3D11CreateDeviceAndSwapChain",
                        D3D11CreateDeviceAndSwapChain(adapter,
                                                      D3D_DRIVER_TYPE_UNKNOWN,
                                                      NULL,
                                                      creation_flags,
                                                      feature_levels,
                                                      ARRAYSIZE(feature_levels),
                                                      D3D11_SDK_VERSION,
                                                      &swapchain_desc,
                                                      &m_Data->swap_chain,
                                                      &m_Data->device,
                                                      NULL,
                                                      &m_Data->context)))
        {
            return false;
        }

        s_Device = m_Data->device;
        s_Context = m_Data->context;

        ID3D11Texture2D* back_buffer;
        if (FailedCheck("Getting backbuffer texture from swapchain",
                        m_Data->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer)))
        {
            return false;
        }

        m_Data->back_buffer.CreateFromTexture(back_buffer);
        m_Data->back_buffer.SetAsActiveTarget();

        D3D11_VIEWPORT view = { .TopLeftX = 0.0f,
                                .TopLeftY = 0.0f,
                                .Width = (f32)m_Window.GetSize().x,
                                .Height = (f32)m_Window.GetSize().y,
                                .MinDepth = 0.0f,
                                .MaxDepth = 0.0f };

        m_Data->context->RSSetViewports(1, &view);

        Window::RegisterResizeCallback(this, [&](u32 width, u32 height) { ResizeBackbuffer(width, height); });

        INFO_LOG("Finished initializing DirectX11 Framework!");
        return true;
    }

    void Framework::BeginFrame(const Vec4f& clear_color)
    {
        m_Data->back_buffer.SetAsActiveTarget();
        m_Data->back_buffer.Clear(clear_color);
    }

    void Framework::EndFrame()
    {
        m_Data->swap_chain->Present(0, 0);
    }

    void Framework::SetBackbufferAsActiveTarget()
    {
        m_Data->back_buffer.SetAsActiveTarget();
    }

    ID3D11Device* Framework::GetDevice()
    {
        return s_Device;
    }

    ID3D11DeviceContext* Framework::GetContext()
    {
        return s_Context;
    }

    void Framework::ResizeBackbuffer(u32 width, u32 height)
    {
        if (width == 0 || height == 0)
            return;

        m_Data->context->OMSetRenderTargets(0, 0, 0);
        m_Data->back_buffer.Release();

        if (FailedCheck("Resizing backbuffer", m_Data->swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)))
            return;

        ID3D11Texture2D* buffer = nullptr;
        if (FailedCheck("Getting buffer", m_Data->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer)))
            return;

        m_Data->back_buffer.CreateFromTexture(buffer);
        m_Data->back_buffer.SetAsActiveTarget();

        VERBOSE_LOG("Resized backbuffer to (%d, %d)", width, height);
    }

    void Framework::BeginEvent(std::string name)
    {
        if (!s_Annot)
        {
            ID3DUserDefinedAnnotation* annot = nullptr;
            GetContext()->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void**)&annot);
            s_Annot = annot;
        }

        // little bit of a hack to convert from std::string to std::wstring using the std library!
        ((ID3DUserDefinedAnnotation*)s_Annot)->BeginEvent(std::filesystem::path(name).wstring().c_str());
    }

    void Framework::EndEvent()
    {
        if (!s_Annot)
            return;
        ((ID3DUserDefinedAnnotation*)s_Annot)->EndEvent();
    }
}  // namespace fw