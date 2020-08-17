#pragma once
#include "Core\Math\Vec4.h"
#include <variant>

namespace fw
{
	struct SpriteCommand
	{
		Vec4f position;
		Vec2f origin;
		Vec2f scale;
		Vec4f color;
		f32 rotation;
		bool world_space = true;
	};

	using RenderCommand = std::variant<SpriteCommand>;
}