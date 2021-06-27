#pragma once
#include "Core\Math\Vec4.h"
#include "TextureID.h"
#include "Camera.h"
#include <variant>

namespace fw
{
	struct SpriteCommand
	{
		TextureID texture;
		Vec4f color;
		Vec2f position;
		Vec2f origin;
		Vec2f scale;
		f32 rotation;
		f32 layer;
		bool world_space = true;
	};

	struct SetCameraCommand
	{
		Camera* camera;
		Mat4f view;
	};

	using RenderCommand = std::variant<SpriteCommand, SetCameraCommand>;
}