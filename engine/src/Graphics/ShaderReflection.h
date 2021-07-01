#pragma once

namespace fw
{
    class ShaderReflection
    {
    public:
        bool Reflect(ComPtr<ID3DBlob> blob);

        void ProcessInputParameters(std::function<void(D3D11_SIGNATURE_PARAMETER_DESC)> visitor);
        void ProcessBoundResources(std::function<void(D3D11_SHADER_INPUT_BIND_DESC)> visitor);

    private:
        ComPtr<ID3D11ShaderReflection> m_Reflection;
        D3D11_SHADER_DESC m_ShaderDesc;
    };
}  // namespace fw