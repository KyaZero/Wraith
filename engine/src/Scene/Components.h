#pragma once
#include "Core/Math/Vec.h"
#include "Graphics/TextureID.h"
#include "Graphics/Camera.h"
#include "ScriptableEntity.h"

namespace fw
{
	struct TransformComponent
	{
		Vec3f position{ 0.f, 0.f, 0.f };
		Vec3f scale{ 1.f, 1.f, 1.f };
		Vec3f rotation{ 0.f, 0.f, 0.f };
	};

	struct TagComponent
	{
		std::string tag;
	};

	struct SpriteComponent
	{
		TextureID texture;
		Vec4f color{ 1, 1, 1, 1 };
		Vec2f origin{ 0, 0 };
		f32 layer{ 0 };
		bool world_space{ true };
	};

	struct CameraComponent
	{
		Camera camera;
		bool primary = true;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};
}