#pragma once

#include "Core/Math/Vec.h"
#include "Graphics/Camera.h"
#include "Graphics/TextureID.h"
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
        using PtrScriptableEntity = std::unique_ptr<ScriptableEntity>;
        using InstantiateFunction = std::function<PtrScriptableEntity()>;

        std::unique_ptr<ScriptableEntity> instance;
        InstantiateFunction InstantiateScript;

        template <typename T, typename... Args>
        void Bind(Args&&... args)
        {
            InstantiateScript = [args...]() { return std::make_unique<T>(args...); };
        }

        void DestroyScript()
        {
            instance.reset();
        }
    };
}  // namespace fw