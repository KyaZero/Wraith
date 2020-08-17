#pragma once
#include "Core\Types.h"

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
		Constant
	};

	class Buffer
	{
	public:
		Buffer();
		Buffer(u32 size, BufferUsage usage, BufferType flags, u32 stride = 0, void* data = nullptr);
		~Buffer();
		void Init(u32 size, BufferUsage usage, BufferType flags, u32 stride = 0, void* data = nullptr);

		void Bind(i32 slot = 0) const;

		template<typename T>
		void SetData(T& data);

		void SetData(void* data, u32 size);
		void* Map();
		void Unmap();
	private:
		struct Data;
		Data* m_Data;
	};

	template<typename T>
	inline void Buffer::SetData(T& data)
	{
		SetData(&data, sizeof(data));
	}
}