#include "Shader.h"
#include "Framework.h"
#include "DXUtil.h"
#include "ShaderReflection.h"
#include "Core\Filewatcher.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace fw
{
	struct Shader::Data
	{
		ComPtr<ID3D11VertexShader> vertex;
		ComPtr<ID3D11PixelShader> pixel;
		ComPtr<ID3D11GeometryShader> geometry;
		ComPtr<ID3D11InputLayout> input_layout;

		ShaderType type;
	};

	HRESULT CompileShader(const std::string& path, const std::string& entry_point, const std::string& profile, ComPtr<ID3DBlob>& blob)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> shader_blob;
		ComPtr<ID3DBlob> error_blob;

		std::wstring w_path(path.begin(), path.end());

		HRESULT hr = D3DCompileFromFile(w_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.c_str(), profile.c_str(), flags, NULL, &shader_blob, &error_blob);

		if (FailedCheck("D3DCompileFromFile: " + path + " | " + entry_point, hr))
		{
			if (error_blob)
				ERROR_LOG("Shader Error:\n%s\n", static_cast<char*>(error_blob->GetBufferPointer()));

			return hr;
		}

		blob = shader_blob;

		return hr;
	}

	DXGI_FORMAT FormatFromComponentType(BYTE mask, D3D_REGISTER_COMPONENT_TYPE component_type)
	{
		if (mask == 1)
		{
			switch (component_type)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				return DXGI_FORMAT_R32_UINT;
			case D3D_REGISTER_COMPONENT_SINT32:
				return DXGI_FORMAT_R32_SINT;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				return DXGI_FORMAT_R32_FLOAT;
			}
		}
		else if (mask <= 3)
		{
			switch (component_type)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				return DXGI_FORMAT_R32G32_UINT;
			case D3D_REGISTER_COMPONENT_SINT32:
				return DXGI_FORMAT_R32G32_SINT;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				return DXGI_FORMAT_R32G32_FLOAT;
			}
		}
		else if (mask <= 7)
		{
			switch (component_type)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				return DXGI_FORMAT_R32G32B32_UINT;
			case D3D_REGISTER_COMPONENT_SINT32:
				return DXGI_FORMAT_R32G32B32_SINT;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			}
		}
		else if (mask <= 15)
		{
			switch (component_type)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				return DXGI_FORMAT_R32G32B32A32_UINT;
			case D3D_REGISTER_COMPONENT_SINT32:
				return DXGI_FORMAT_R32G32B32A32_SINT;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	HRESULT CreateInputLayout(ComPtr<ID3DBlob> vs_blob, ComPtr<ID3D11InputLayout>& input_layout)
	{
		ShaderReflection shaderReflection;
		shaderReflection.Reflect(vs_blob);

		std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desciptions;

		shaderReflection.ProcessInputParameters([&](auto param_desc)
			{
				auto element_desc = D3D11_INPUT_ELEMENT_DESC{
					.SemanticName = param_desc.SemanticName,
					.SemanticIndex = param_desc.SemanticIndex,
					.Format = FormatFromComponentType(param_desc.Mask, param_desc.ComponentType),
					.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
				};

				input_element_desciptions.push_back(element_desc);
			});

		HRESULT hr = Framework::GetDevice()->CreateInputLayout(input_element_desciptions.data(), static_cast<UINT>(input_element_desciptions.size()), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &input_layout);

		if (FailedCheck("Creating Input Layout", hr))
		{
			return hr;
		}

		return hr;
	}

	void CreateResourceBindings(ComPtr<ID3DBlob> ps_blob)
	{
		ShaderReflection shaderReflection;
		shaderReflection.Reflect(ps_blob);

		shaderReflection.ProcessBoundResources([](auto resource_desc)
		{
		});
	}

	Shader::Shader()
	{
		m_Data = std::make_shared<Data>();
	}

	Shader::Shader(const Shader& other)
	{
		operator=(other);
	}

	Shader::Shader(Shader&& other)
	{
		operator=(std::forward<Shader>(other));
	}

	Shader::~Shader()
	{
	}

	Shader& Shader::operator=(const Shader& other)
	{
		m_Data = other.m_Data;
		return *this;
	}

	Shader& Shader::operator=(Shader&& other)
	{
		m_Data = other.m_Data;
		other.m_Data = nullptr;
		return *this;
	}

	bool Shader::Load(std::underlying_type_t<ShaderType> shader_type, const std::string& path)
	{
		m_Data->type = static_cast<ShaderType>(shader_type);

		const auto compile_vertex = [=]()
		{
			if (!(m_Data->type & ShaderType::Vertex))
				return;

			ComPtr<ID3DBlob> vs_blob;

			if (FailedCheck(CompileShader(path, "VSMain", "vs_5_0", vs_blob)))
				return;

			auto* device = Framework::GetDevice();
			HRESULT hr = device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), NULL, &m_Data->vertex);
			if (FailedCheck("Creating Vertex Shader", hr))
			{
				hr = device->GetDeviceRemovedReason();
				return;
			}

			if (FailedCheck("Creating Input Layout", CreateInputLayout(vs_blob, m_Data->input_layout)))
				return;

			return;
		};

		const auto compile_geometry = [=]()
		{
			if (!(m_Data->type & ShaderType::Geometry))
				return;

			ComPtr<ID3DBlob> gs_blob;

			if (FailedCheck(CompileShader(path, "GSMain", "gs_5_0", gs_blob)))
				return;

			auto* device = Framework::GetDevice();
			HRESULT hr = device->CreateGeometryShader(gs_blob->GetBufferPointer(), gs_blob->GetBufferSize(), NULL, &m_Data->geometry);
			if (FailedCheck("Creating Geometry Shader", hr))
			{
				hr = device->GetDeviceRemovedReason();
				return;
			}
		};

		const auto compile_pixel = [=]()
		{
			if (!(m_Data->type & ShaderType::Pixel))
				return;

			ComPtr<ID3DBlob> ps_blob;

			if (FailedCheck(CompileShader(path, "PSMain", "ps_5_0", ps_blob)))
				return;

			auto* device = Framework::GetDevice();
			HRESULT hr = device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), NULL, &m_Data->pixel);
			if (FailedCheck("Creating Pixel Shader", hr))
			{
				hr = device->GetDeviceRemovedReason();
				return;
			}

			CreateResourceBindings(ps_blob);
		};

		compile_vertex();
		compile_geometry();
		compile_pixel();

		Filewatcher::Get()->Watch(path, [=]() {
			compile_vertex();
			compile_geometry();
			compile_pixel();
		});

		return true;
	}

	void Shader::Bind()
	{
		auto* context = Framework::GetContext();

		if (m_Data->type & ShaderType::Vertex && m_Data->vertex)
		{
			context->IASetInputLayout(m_Data->input_layout.Get());
			context->VSSetShader(m_Data->vertex.Get(), 0, 0);
		}

		if (m_Data->type & ShaderType::Geometry && m_Data->geometry)
		{
			context->GSSetShader(m_Data->geometry.Get(), 0, 0);
		}

		if (m_Data->type & ShaderType::Pixel && m_Data->pixel)
		{
			context->PSSetShader(m_Data->pixel.Get(), 0, 0);
		}
	}

	void Shader::Unbind()
	{
		auto* context = Framework::GetContext();

		if (m_Data->type & ShaderType::Vertex && m_Data->vertex)
		{
			context->IASetInputLayout(nullptr);
			context->VSSetShader(nullptr, 0, 0);
		}

		if (m_Data->type & ShaderType::Geometry && m_Data->geometry)
		{
			context->GSSetShader(nullptr, 0, 0);
		}

		if (m_Data->type & ShaderType::Pixel && m_Data->pixel)
		{
			context->PSSetShader(nullptr, 0, 0);
		}
	}
}