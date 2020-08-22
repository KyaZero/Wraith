#include "Buffer.h"
#include "Framework.h"
#include "DXUtil.h"
#include <d3d11.h>

namespace fw
{
	struct Buffer::Data
	{
		ID3D11Buffer* buffer;
		ID3D11ShaderResourceView* resource_view;
		BufferType bindFlags;
		u32 stride;
	};

	Buffer::Buffer() : m_Data(nullptr)
	{
	}

	Buffer::Buffer(u32 size, BufferUsage usage, BufferType flags, u32 stride, void* data)
	{
		Init(size, usage, flags, stride, data);
	}

	Buffer::~Buffer()
	{
		if (m_Data)
		{
			if (m_Data->buffer)
				SafeRelease(&m_Data->buffer);
			if (m_Data->resource_view)
				SafeRelease(&m_Data->resource_view);
		}
	}

	void Buffer::Init(u32 size, BufferUsage usage, BufferType flags, u32 stride, void* data)
	{
		m_Data = std::make_unique<Data>();

		D3D11_BUFFER_DESC desc = { };
		desc.ByteWidth = size;

		switch (usage)
		{
		case BufferUsage::Immutable:
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			break;
		case BufferUsage::Dynamic:
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		}

		switch (flags)
		{
		case BufferType::Vertex:
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			break;
		case BufferType::Index:
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			break;
		case BufferType::Constant:
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			break;
		case BufferType::Structured:
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desc.StructureByteStride = stride;
			break;
		}

		if (flags == BufferType::Constant && usage == BufferUsage::Dynamic && size % 16 != 0)
		{
			FATAL_LOG("Misaligned constant buffer!");
		}

		D3D11_SUBRESOURCE_DATA subresource = { };
		subresource.pSysMem = data;

		if (FailedCheck("Creating Buffer", Framework::GetDevice()->CreateBuffer(&desc, (data) ? &subresource : nullptr, &m_Data->buffer)))
		{
			ASSERT_LOG(false);
		}

		if (flags == BufferType::Structured)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.ElementWidth = size / stride;
			Framework::GetDevice()->CreateShaderResourceView(m_Data->buffer, &srvDesc, &m_Data->resource_view);
		}

		m_Data->bindFlags = flags;
		m_Data->stride = stride;
	}

	void Buffer::Bind(i32 slot) const
	{
		ASSERT_LOG(m_Data, "Buffer not Inititalized!");

		auto* context = Framework::GetContext();
		const u32 offset = 0;

		switch (m_Data->bindFlags)
		{
		case BufferType::Vertex:
			context->IASetVertexBuffers(slot, 1, &m_Data->buffer, (UINT*)&m_Data->stride, &offset);
			break;

		case BufferType::Index:
			context->IASetIndexBuffer(m_Data->buffer, DXGI_FORMAT_R32_UINT, 0);
			break;

		case BufferType::Constant:
			context->VSSetConstantBuffers(slot, 1, &m_Data->buffer);
			context->PSSetConstantBuffers(slot, 1, &m_Data->buffer);
			context->GSSetConstantBuffers(slot, 1, &m_Data->buffer);
			break;
		case BufferType::Structured:
			context->VSSetShaderResources(1, 1, &m_Data->resource_view);
			break;
		default:
			ASSERT_LOG(false, "No implemented BufferType");
			break;
		}
	}

	void Buffer::SetData(void* data, u32 size)
	{
		memcpy(Map(), data, size);
		Unmap();
	}

	void* Buffer::Map()
	{
		if (!m_Data)
		{
			ERROR_LOG("Buffer not Inititalized!");
			return nullptr;
		}
		D3D11_MAPPED_SUBRESOURCE subres;
		memset(&subres, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

		if (FailedCheck("Mapping buffer", Framework::GetContext()->Map(m_Data->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subres)))
			return nullptr;

		return subres.pData;
	}

	void Buffer::Unmap()
	{
		if (!m_Data)
		{
			ERROR_LOG("Buffer not Inititalized!");
			return;
		}
		Framework::GetContext()->Unmap(m_Data->buffer, 0);
	}
}