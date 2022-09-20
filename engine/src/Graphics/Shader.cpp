#include "Shader.h"

#include <comdef.h>

#include "Core/Filewatcher.h"
#include "DXUtil.h"
#include "Framework.h"
#include "ShaderReflection.h"

namespace Wraith
{
    struct Shader::Data
    {
        // ComPtr<ID3D11VertexShader> vertex;
        // ComPtr<ID3D11PixelShader> pixel;
        // ComPtr<ID3D11GeometryShader> geometry;
        // ComPtr<ID3D11InputLayout> input_layout;
    };

    //    HRESULT InternalCompileShader(const std::string& path,
    //                                  const std::string& entry_point,
    //                                  const std::string& profile,
    //                                  ComPtr<ID3DBlob>& blob)
    //    {
    //        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
    //#ifdef _DEBUG
    //        flags |= D3DCOMPILE_DEBUG;
    //#endif
    //        ComPtr<ID3DBlob> shader_blob;
    //        ComPtr<ID3DBlob> error_blob;
    //
    //        std::wstring w_path(path.begin(), path.end());
    //
    //        HRESULT hr = D3DCompileFromFile(w_path.c_str(),
    //                                        nullptr,
    //                                        D3D_COMPILE_STANDARD_FILE_INCLUDE,
    //                                        entry_point.c_str(),
    //                                        profile.c_str(),
    //                                        flags,
    //                                        NULL,
    //                                        &shader_blob,
    //                                        &error_blob);
    //
    //        if (FailedCheck("D3DCompileFromFile: " + path + " | " + entry_point, hr))
    //        {
    //            if (error_blob)
    //            {
    //                ERROR_LOG("Shader Error:\n{}\n", static_cast<char*>(error_blob->GetBufferPointer()));
    //            }
    //            else
    //            {
    //                _com_error err(hr);
    //                LPCTSTR errMsg = err.ErrorMessage();
    //                ERROR_LOG("Shader Error:\n{}\n", std::filesystem::path(errMsg).string().c_str());
    //            }
    //
    //            return hr;
    //        }
    //
    //        blob = shader_blob;
    //
    //        return hr;
    //    }
    //
    //    DXGI_FORMAT FormatFromComponentType(BYTE mask, D3D_REGISTER_COMPONENT_TYPE component_type)
    //    {
    //        if (mask == 1)
    //        {
    //            switch (component_type)
    //            {
    //            case D3D_REGISTER_COMPONENT_UINT32:
    //                return DXGI_FORMAT_R32_UINT;
    //            case D3D_REGISTER_COMPONENT_SINT32:
    //                return DXGI_FORMAT_R32_SINT;
    //            case D3D_REGISTER_COMPONENT_FLOAT32:
    //                return DXGI_FORMAT_R32_FLOAT;
    //            }
    //        }
    //        else if (mask <= 3)
    //        {
    //            switch (component_type)
    //            {
    //            case D3D_REGISTER_COMPONENT_UINT32:
    //                return DXGI_FORMAT_R32G32_UINT;
    //            case D3D_REGISTER_COMPONENT_SINT32:
    //                return DXGI_FORMAT_R32G32_SINT;
    //            case D3D_REGISTER_COMPONENT_FLOAT32:
    //                return DXGI_FORMAT_R32G32_FLOAT;
    //            }
    //        }
    //        else if (mask <= 7)
    //        {
    //            switch (component_type)
    //            {
    //            case D3D_REGISTER_COMPONENT_UINT32:
    //                return DXGI_FORMAT_R32G32B32_UINT;
    //            case D3D_REGISTER_COMPONENT_SINT32:
    //                return DXGI_FORMAT_R32G32B32_SINT;
    //            case D3D_REGISTER_COMPONENT_FLOAT32:
    //                return DXGI_FORMAT_R32G32B32_FLOAT;
    //            }
    //        }
    //        else if (mask <= 15)
    //        {
    //            switch (component_type)
    //            {
    //            case D3D_REGISTER_COMPONENT_UINT32:
    //                return DXGI_FORMAT_R32G32B32A32_UINT;
    //            case D3D_REGISTER_COMPONENT_SINT32:
    //                return DXGI_FORMAT_R32G32B32A32_SINT;
    //            case D3D_REGISTER_COMPONENT_FLOAT32:
    //                return DXGI_FORMAT_R32G32B32A32_FLOAT;
    //            }
    //        }
    //
    //        return DXGI_FORMAT_UNKNOWN;
    //    }

    // HRESULT CreateInputLayout(ComPtr<ID3DBlob> vs_blob, ComPtr<ID3D11InputLayout>& input_layout)
    //{
    //    ShaderReflection shaderReflection;
    //    shaderReflection.Reflect(vs_blob);

    //    std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desciptions;

    //    shaderReflection.ProcessInputParameters([&](auto param_desc) {
    //        auto element_desc = D3D11_INPUT_ELEMENT_DESC{
    //            .SemanticName = param_desc.SemanticName,
    //            .SemanticIndex = param_desc.SemanticIndex,
    //            .Format = FormatFromComponentType(param_desc.Mask, param_desc.ComponentType),
    //            .AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
    //        };

    //        input_element_desciptions.push_back(element_desc);
    //    });

    //    HRESULT hr = Framework::GetDevice().CreateInputLayout(input_element_desciptions.data(),
    //                                                          static_cast<UINT>(input_element_desciptions.size()),
    //                                                          vs_blob->GetBufferPointer(),
    //                                                          vs_blob->GetBufferSize(),
    //                                                          &input_layout);

    //    if (FailedCheck("Creating Input Layout", hr))
    //    {
    //        return hr;
    //    }

    //    return hr;
    //}

    // void CreateResourceBindings(ComPtr<ID3DBlob> ps_blob)
    //{
    //    ShaderReflection shaderReflection;
    //    shaderReflection.Reflect(ps_blob);

    //    shaderReflection.ProcessBoundResources([](auto resource_desc) {});
    //}

    Shader::Shader()
    {
        if (!m_Data)
            m_Data = std::make_unique<Data>();
    }

    Shader::Shader(std::underlying_type_t<ShaderType> shader_type, const std::string& path)
        : Shader()
    {
        Load(shader_type, path);
    }

    Shader::Shader(const Shader& other)
        : Shader()
    {
        operator=(other);
    }

    Shader::~Shader() { }

    Shader& Shader::operator=(const Shader& other)
    {
        // m_Data->vertex = other.m_Data->vertex;
        // m_Data->pixel = other.m_Data->pixel;
        // m_Data->geometry = other.m_Data->geometry;
        // m_Data->input_layout = other.m_Data->input_layout;
        m_Type = other.m_Type;
        m_Path = other.m_Path;
        m_IsValid = other.m_IsValid;

        Filewatcher::Get()->Watch(m_Path, [&]() { CompileShader(); });

        return *this;
    }

    bool Shader::Load(std::underlying_type_t<ShaderType> shader_type, const std::string& path)
    {
        m_Type = static_cast<ShaderType>(shader_type);
        m_Path = path;
        m_IsValid = CompileShader();

        if (m_IsValid)
            Filewatcher::Get()->Watch(m_Path, [&]() { CompileShader(); });

        return m_IsValid;
    }

    void Shader::Bind() const
    {
        // auto& context = Framework::GetContext();

        if (m_Type & ShaderType::Vertex /*&& m_Data->vertex*/)
        {
            // context.IASetInputLayout(m_Data->input_layout.Get());
            // context.VSSetShader(m_Data->vertex.Get(), 0, 0);
        }

        if (m_Type & ShaderType::Geometry /*&& m_Data->geometry*/)
        {
            // context.GSSetShader(m_Data->geometry.Get(), 0, 0);
        }

        if (m_Type & ShaderType::Pixel /*&& m_Data->pixel*/)
        {
            // context.PSSetShader(m_Data->pixel.Get(), 0, 0);
        }
    }

    void Shader::Unbind() const
    {
        // auto& context = Framework::GetContext();

        if (m_Type & ShaderType::Vertex /*&& m_Data->vertex*/)
        {
            // context.IASetInputLayout(nullptr);
            // context.VSSetShader(nullptr, 0, 0);
        }

        if (m_Type & ShaderType::Geometry /*&& m_Data->geometry*/)
        {
            // context.GSSetShader(nullptr, 0, 0);
        }

        if (m_Type & ShaderType::Pixel /*&& m_Data->pixel*/)
        {
            // context.PSSetShader(nullptr, 0, 0);
        }
    }

    bool Shader::IsValid() const { return m_IsValid; }

    Shader::ShaderType Shader::GetType() const { return m_Type; }

    std::string Shader::GetName() const { return std::filesystem::path(m_Path).filename().string(); }

    bool Shader::CompileShader() const
    {
        // ComPtr<ID3D11VertexShader> vertex_shader;
        // ComPtr<ID3D11InputLayout> input_layout;
        // ComPtr<ID3D11GeometryShader> geometry_shader;
        // ComPtr<ID3D11PixelShader> pixel_shader;

        const auto compile_vertex = [&]() -> bool {
            if (!(m_Type & ShaderType::Vertex))
                return true;

            // auto& device = Framework::GetDevice();
            // ComPtr<ID3DBlob> vs_blob;

            // if (FailedCheck(InternalCompileShader(m_Path, "VSMain", "vs_5_0", vs_blob)))
            //    return false;

            // HRESULT hr =
            //    device.CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), NULL, &vertex_shader);
            // if (FailedCheck("Creating Vertex Shader", hr))
            //{
            //    hr = device.GetDeviceRemovedReason();
            //    return false;
            //}

            // if (FailedCheck("Creating Input Layout", CreateInputLayout(vs_blob, input_layout)))
            //    return false;

            return true;
        };

        const auto compile_geometry = [&]() -> bool {
            if (!(m_Type & ShaderType::Geometry))
                return true;

            // ComPtr<ID3DBlob> gs_blob;

            // if (FailedCheck(InternalCompileShader(m_Path, "GSMain", "gs_5_0", gs_blob)))
            //    return false;

            // auto& device = Framework::GetDevice();
            // HRESULT hr = device.CreateGeometryShader(
            //    gs_blob->GetBufferPointer(), gs_blob->GetBufferSize(), NULL, &geometry_shader);
            // if (FailedCheck("Creating Geometry Shader", hr))
            //{
            //    hr = device.GetDeviceRemovedReason();
            //    return false;
            //}

            return true;
        };

        const auto compile_pixel = [&]() -> bool {
            if (!(m_Type & ShaderType::Pixel))
                return true;

            // ComPtr<ID3DBlob> ps_blob;

            // if (FailedCheck(InternalCompileShader(m_Path, "PSMain", "ps_5_0", ps_blob)))
            //    return false;

            // auto& device = Framework::GetDevice();
            // HRESULT hr =
            //    device.CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), NULL, &pixel_shader);
            // if (FailedCheck("Creating Pixel Shader", hr))
            //{
            //    hr = device.GetDeviceRemovedReason();
            //    return false;
            //}

            // CreateResourceBindings(ps_blob);

            return true;
        };

        // if any compiles fail, we wanna return false so as to keep the previous shader files.
        if (!(compile_vertex() && compile_geometry() && compile_pixel()))
            return false;

        // m_Data->vertex = vertex_shader;
        // m_Data->input_layout = input_layout;
        // m_Data->geometry = geometry_shader;
        // m_Data->pixel = pixel_shader;

        return true;
    }
}  // namespace Wraith