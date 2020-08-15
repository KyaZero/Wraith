#include "ShaderReflection.h"
#include "DXUtil.h"

namespace fw
{
	bool ShaderReflection::Reflect(ComPtr<ID3DBlob> blob)
	{
		if (FailedCheck("Getting Shader Reflection Interface", D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_Reflection))))
		{
			return false;
		}

		m_Reflection->GetDesc(&m_ShaderDesc);

		return true;
	}

	void ShaderReflection::ProcessInputParameters(std::function<void(D3D11_SIGNATURE_PARAMETER_DESC)> visitor)
	{
		for (u32 i = 0; i < m_ShaderDesc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC param_desc;
			m_Reflection->GetInputParameterDesc(i, &param_desc);

			visitor(param_desc);
		}
	}

	void ShaderReflection::ProcessBoundResources(std::function<void(D3D11_SHADER_INPUT_BIND_DESC)> visitor)
	{
		for (u32 i = 0; i < m_ShaderDesc.InputParameters; ++i)
		{
			D3D11_SHADER_INPUT_BIND_DESC resource_desc;
			m_Reflection->GetResourceBindingDesc(i, &resource_desc);

			visitor(resource_desc);
		}
	}
}

