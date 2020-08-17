#pragma once
#include "Core/Math/Vec.h"
#include "Graphics/TextureID.h"

namespace fw
{
	struct TransformComponent
	{
		Vec2f position{ 0.f,0.f };
		Vec2f scale{ 1.f,1.f };
		f32 rotation{ 0.f };
	};

	struct TagComponent
	{
		std::string tag;
	};

	struct SpriteComponent
	{
		TextureID texture;
		Vec4f color{ 1,1,1,1 };
		Vec2f origin{ 0,0 };
		f32 layer{ 0 };
		bool world_space{ true };
	};
}