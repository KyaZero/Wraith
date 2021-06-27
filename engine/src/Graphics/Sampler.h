#pragma once

#include <memory>

#include "Core/Math/Vec4.h"

namespace fw
{
	class Sampler
	{
	public:
		enum class Filter
		{
			Linear,
			Point,
			Anisotropic
		};

		enum class Address
		{
			Clamp,
			Wrap,
			Mirror,
			Border
		};

		Sampler();
		Sampler(Filter filter, Address address, Vec4f border = Vec4f());
		~Sampler();

		void Init(Filter filter, Address address, Vec4f border = Vec4f());
		void Bind(u32 slot);
		void Unbind(u32 slot);

	private:
		struct Data;
		std::unique_ptr<Data> m_Data;
	};
}  // namespace fw