#include "Sampler.h"

#include "DXUtil.h"
#include "Framework.h"

namespace fw
{
    struct Sampler::Data
    {
        ComPtr<ID3D11SamplerState> sampler;
    };

    Sampler::Sampler()
    { }

    Sampler::Sampler(Filter filter, Address address, Vec4f border)
    {
        Init(filter, address, border);
    }

    Sampler::~Sampler()
    { }

    void Sampler::Init(Filter filter, Address address, Vec4f border)
    {
        m_Data = std::make_unique<Data>();

        D3D11_SAMPLER_DESC desc;
        switch (filter)
        {
        case Sampler::Filter::Linear:
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            break;
        case Sampler::Filter::Point:
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            break;
        case Sampler::Filter::Anisotropic:
            desc.Filter = D3D11_FILTER_ANISOTROPIC;
            break;
        }

        switch (address)
        {
        case Sampler::Address::Clamp:
            desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            break;
        case Sampler::Address::Wrap:
            desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            break;
        case Sampler::Address::Mirror:
            desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
            break;
        case Sampler::Address::Border:
            desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
            break;
        }

        desc.MipLODBias = 0.0f;
        desc.MaxAnisotropy = 16;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.BorderColor[0] = border.x;
        desc.BorderColor[1] = border.y;
        desc.BorderColor[2] = border.z;
        desc.BorderColor[3] = border.w;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        FailedCheck("Creating Sampler State", Framework::GetDevice()->CreateSamplerState(&desc, &m_Data->sampler));
    }

    void Sampler::Bind(u32 slot)
    {
        Framework::GetContext()->PSSetSamplers(slot, 1, m_Data->sampler.GetAddressOf());
    }

    void Sampler::Unbind(u32 slot)
    {
        ID3D11SamplerState* sampler = nullptr;
        Framework::GetContext()->PSSetSamplers(slot, 1, &sampler);
    }
}  // namespace fw