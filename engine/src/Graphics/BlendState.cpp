#include "BlendState.h"

#include "Graphics/DXUtil.h"
#include "Graphics/Framework.h"

namespace Wraith
{
    struct BlendState::Data
    {
        ComPtr<ID3D11BlendState> blend_state;
    };

    BlendState::BlendState()
    { }
    BlendState::~BlendState()
    { }

    bool BlendState::Create(u8 src_rgb, u8 dest_rgb, u8 src_alpha, u8 dest_alpha, u8 op_rgb, u8 op_alpha)
    {
        m_Data = std::make_unique<Data>();

        D3D11_BLEND_DESC blend_desc{
            .RenderTarget = { {
                .BlendEnable = TRUE,
                .SrcBlend = static_cast<D3D11_BLEND>(src_rgb),
                .DestBlend = static_cast<D3D11_BLEND>(dest_rgb),
                .BlendOp = static_cast<D3D11_BLEND_OP>(op_rgb),
                .SrcBlendAlpha = static_cast<D3D11_BLEND>(src_alpha),
                .DestBlendAlpha = static_cast<D3D11_BLEND>(dest_alpha),
                .BlendOpAlpha = static_cast<D3D11_BLEND_OP>(op_alpha),
                .RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL,
            } },
        };

        return !FailedCheck(Framework::GetDevice()->CreateBlendState(&blend_desc, &m_Data->blend_state));
    }
    void BlendState::Bind()
    {
        const Vec4f blend_factor;
        Framework::GetContext()->OMSetBlendState(m_Data->blend_state.Get(), &blend_factor.x, 0xFFFFFFFF);
    }
    void BlendState::Unbind()
    {
        Framework::GetContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    }
}  // namespace fw