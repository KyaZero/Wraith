#pragma once
#include "Core\Math\Vec4.h"
#include "TextureID.h"
#include <variant>

namespace fw
{
	struct SpriteCommand
	{
		TextureID texture;
		Vec4f position;
		Vec4f color;
		Vec2f origin;
		Vec2f scale;
		f32 rotation;
		bool world_space = true;
	};

	using RenderCommand = std::variant<SpriteCommand>;
}