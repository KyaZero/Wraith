#include "Framework.h"
#include "DXUtil.h"
#include "Texture.h"
#include <d3d11.h>

namespace fw
{
	//for getters, should figure out a better way to do this
	static ID3D11Device* s_Device;
	static ID3D11DeviceContext* s_Context;

	struct Framework::Data
	{
		IDXGISwapChain* swap_chain;
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		Texture back_buffer;
		std::vector<IDXGIAdapter*> adapters;
	};

	Framework::Framework()
	{
		m_Data = new Data;
	}

	Framework::~Framework()
	{
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

	bool Framework::Init(Window* window)
	{
		if (!window)
			return false;

		m_Window = window;

		m_Data->adapters = EnumerateAdapters();

		IDXGIAdapter* adapter = nullptr;
		for (auto* a : m_Data->adapters)
		{
			DXGI_ADAPTER_DESC adapterDescription;
			a->GetDesc(&adapterDescription);

			INFO_LOG("Found adapter: %ls VRAM: %umb", adapterDescription.Description, adapterDescription.DedicatedVideoMemory / (1024 * 1024));

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

		D3D_FEATURE_LEVEL feature_levels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_1
		};

		DXGI_SWAP_CHAIN_DESC swapchain_desc = { 0 };
		swapchain_desc.BufferCount = 1;
		swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		swapchain_desc.OutputWindow = (HWND)m_Window->GetHandle();
		swapchain_desc.SampleDesc.Count = 1;
		swapchain_desc.Windowed = true;

		if (FailedCheck("D3D11CreateDeviceAndSwapChain", D3D11CreateDeviceAndSwapChain(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, creation_flags, feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, &swapchain_desc, &m_Data->swap_chain, &m_Data->device, NULL, &m_Data->context)))
		{
			return false;
		}

		s_Device = m_Data->device;
		s_Context = m_Data->context;

		ID3D11Texture2D* back_buffer;
		if (FailedCheck("Getting backbuffer texture from swapchain", m_Data->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer)))
		{
			return false;
		}

		m_Data->back_buffer.CreateFromTexture(back_buffer);
		m_Data->back_buffer.SetAsActiveTarget();

		D3D11_VIEWPORT view = {
			.TopLeftX = 0.0f,
			.TopLeftY = 0.0f,
			.Width = (f32)m_Window->GetSize().x,
			.Height = (f32)m_Window->GetSize().y,
			.MinDepth = 0.0f,
			.MaxDepth = 0.0f
		};

		m_Data->context->RSSetViewports(1, &view);

		m_Window->Subscribe(Event::Resized, [&](const Event& e) {
			ResizeBackbuffer();
		});

		INFO_LOG("Finished initializing DirectX11 Framework!");
		return true;
	}

	void Framework::BeginFrame(const Vec4f& clear_color)
	{
		m_Data->back_buffer.Clear(clear_color);
	}

	void Framework::EndFrame()
	{
		m_Data->swap_chain->Present(0, 0);
	}

	ID3D11Device* Framework::GetDevice()
	{
		return s_Device;
	}

	ID3D11DeviceContext* Framework::GetContext()
	{
		return s_Context;
	}

	void Framework::ResizeBackbuffer()
	{
		if (m_Window->GetSize().x == 0 || m_Window->GetSize().y == 0)
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

		VERBOSE_LOG("Resized backbuffer to (%d, %d)", m_Window->GetSize().x, m_Window->GetSize().y);
	}
}