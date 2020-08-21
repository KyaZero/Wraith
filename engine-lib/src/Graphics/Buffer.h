#pragma once
#include "Core\Types.h"
#include <memory>

namespace fw
{
	enum class BufferUsage
	{
		Immutable,
		Dynamic
	};

	enum class BufferType
	{
		Vertex,
		Index,
		Constant,
		Structured
	};

	class Buffer
	{
	public:
		Buffer();
		Buffer(u32 size, BufferUsage usage, BufferType flags, u32 stride = 0, void* data = nullptr);
		virtual ~Buffer();
		virtual void Init(u32 size, BufferUsage usage, BufferType flags, u32 stride = 0, void* data = nullptr);

		virtual void Bind(i32 slot = 0) const;

		template<typename T>
		void SetData(T& data);

		virtual void SetData(void* data, u32 size);
		virtual void* Map();
		virtual void Unmap();
	protected:
		struct Data;
		std::unique_ptr<Data> m_Data;
	};

	template<typename T>
	inline void Buffer::SetData(T& data)
	{
		SetData(&data, sizeof(data));
	}
}