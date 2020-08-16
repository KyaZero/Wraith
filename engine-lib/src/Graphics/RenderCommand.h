#pragma once
#include "..\Core\Math\Vec4.h"
#include <variant>

namespace fw
{
	struct SpriteCommand
	{
		Vec4f position;
		Vec4f uv;
		Vec4f color;
	};

	using RenderCommand = std::variant<SpriteCommand>;
}