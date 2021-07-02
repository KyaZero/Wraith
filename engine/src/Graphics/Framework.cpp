#include "Framework.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "DXUtil.h"
#include "Texture.h"

namespace fw
{
    // for getters, should figure out a better way to do this
    static ID3D11Device* s_Device;
    static ID3D11DeviceContext* s_Context;
    static ID3D11Debug* s_DeviceDebug;
    static ID3DUserDefinedAnnotation* s_Annot;

    struct Framework::Data
    {
        ComPtr<IDXGISwapChain> swap_chain;
        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11DeviceContext> context;
        ComPtr<ID3D11RenderTargetView> back_buffer;
        std::vector<ComPtr<IDXGIAdapter>> adapters;
    };

    Framework::Framework(Window& window)
        : m_Window(window)
    {
        m_Data = std::make_unique<Data>();
    }

    Framework::~Framework()
    {
        Window::UnregisterResizeCallback(this);

        m_Data->context->ClearState();
        m_Data->context->Flush();
        ReportLiveObjects();
    }

    std::vector<ComPtr<IDXGIAdapter>> EnumerateAdapters()
    {
        ComPtr<IDXGIFactory> factory;
        ComPtr<IDXGIAdapter> adapter;
        std::vector<ComPtr<IDXGIAdapter>> adapters;

        if (FailedCheck("Gathering adapters", CreateDXGIFactory(IID_PPV_ARGS(&factory))))
        {
            return adapters;
        }

        for (u32 i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            adapters.push_back(adapter);
        }

        return adapters;
    }

    bool Framework::Init()
    {
        m_Data->adapters = EnumerateAdapters();

        ComPtr<IDXGIAdapter> adapter;
        for (const auto& a : m_Data->adapters)
        {
            DXGI_ADAPTER_DESC adapterDescription;
            a->GetDesc(&adapterDescription);

            INFO_LOG("Found adapter: {} VRAM: {}",
                     std::filesystem::path(adapterDescription.Description).string(),
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
                        D3D11CreateDeviceAndSwapChain(adapter.Get(),
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

        m_Data->device->QueryInterface(IID_PPV_ARGS(&s_DeviceDebug));

        s_Device = m_Data->device.Get();
        s_Context = m_Data->context.Get();

        CreateBackbufferRTV();

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
        SetBackbufferAsActiveTarget();
        m_Data->context->ClearRenderTargetView(m_Data->back_buffer.Get(), &clear_color.r);
    }

    void Framework::EndFrame()
    {
        m_Data->swap_chain->Present(0, 0);
    }

    void Framework::SetBackbufferAsActiveTarget()
    {
        m_Data->context->OMSetRenderTargets(1, m_Data->back_buffer.GetAddressOf(), nullptr);
    }

    ID3D11Device* Framework::GetDevice()
    {
        return s_Device;
    }

    ID3D11DeviceContext* Framework::GetContext()
    {
        return s_Context;
    }

    void Framework::ReportLiveObjects()
    {
        if (s_DeviceDebug)
            s_DeviceDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    }

    void Framework::CreateBackbufferRTV()
    {
        ComPtr<ID3D11Texture2D> back_buffer;
        if (FailedCheck("Getting backbuffer texture from swapchain",
                        m_Data->swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer))))
            return;

        if (FailedCheck("Creating backbuffer RTV",
                        m_Data->device->CreateRenderTargetView(back_buffer.Get(), nullptr, &m_Data->back_buffer)))
            return;
    }

    void Framework::ResizeBackbuffer(u32 width, u32 height)
    {
        if (width == 0 || height == 0)
            return;

        m_Data->context->ClearState();
        m_Data->back_buffer.Reset();

        if (FailedCheck("Resizing backbuffer", m_Data->swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)))
            return;

        CreateBackbufferRTV();

        VERBOSE_LOG("Resized backbuffer to ({}, {})", width, height);
    }

    void Framework::BeginEvent(std::string name)
    {
        if (!s_Annot)
        {
            GetContext()->QueryInterface(IID_PPV_ARGS(&s_Annot));
        }

        // little bit of a hack to convert from std::string to std::wstring using the std library!
        s_Annot->BeginEvent(std::filesystem::path(name).wstring().c_str());
    }

    void Framework::EndEvent()
    {
        if (!s_Annot)
            return;
        s_Annot->EndEvent();
    }
}  // namespace fw