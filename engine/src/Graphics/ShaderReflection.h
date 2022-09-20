#pragma once

namespace Wraith
{
    class ShaderReflection
    {
    public:
        bool Reflect(ComPtr<ID3DBlob> blob);

        void ProcessInputParameters(std::function<void(D3D11_SIGNATURE_PARAMETER_DESC)> visitor) const;
        void ProcessBoundResources(std::function<void(D3D11_SHADER_INPUT_BIND_DESC)> visitor) const;

    private:
        ComPtr<ID3D11ShaderReflection> m_Reflection;
        D3D11_SHADER_DESC m_ShaderDesc;
    };
}  // namespace Wraith