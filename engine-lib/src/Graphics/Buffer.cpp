#include "Buffer.h"
#include "Framework.h"
#include "DXUtil.h"
#include <d3d11.h>

namespace fw
{
	struct Buffer::Data
	{
		ID3D11Buffer* buffer;
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
		SafeRelease(&m_Data->buffer);
		delete m_Data;
	}

	void Buffer::Init(u32 size, BufferUsage usage, BufferType flags, u32 stride, void* data)
	{
		m_Data = new Data;

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
		}

		if (flags == BufferType::Constant && usage == BufferUsage::Dynamic && size % 16 == 0)
		{
			FATAL_LOG("Misaligned constant buffer!");
		}

		D3D11_SUBRESOURCE_DATA subresource = { };
		subresource.pSysMem = data;

		FailedCheck("Creating buffer", Framework::GetDevice()->CreateBuffer(&desc, &subresource, &m_Data->buffer));

		m_Data->bindFlags = flags;
		m_Data->stride = stride;
	}

	void Buffer::Bind(i32 slot) const
	{
		if (!m_Data)
		{
			ERROR_LOG("Buffer not Inititalized!");
			return;
		}

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