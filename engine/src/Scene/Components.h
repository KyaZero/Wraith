#pragma once

#include <dubu_serialize/dubu_serialize.h>

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

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> position >> scale >> rotation;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << position << scale << rotation;
        }
    };

    struct TagComponent
    {
        std::string tag;

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> tag;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << tag;
        }
    };

    struct SpriteComponent
    {
        TextureID texture;
        Vec4f color{ 1, 1, 1, 1 };
        Vec2f origin{ 0, 0 };
        f32 layer{ 0 };
        bool world_space{ true };

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> texture >> color >> origin >> layer >> world_space;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << texture << color << origin << layer << world_space;
        }
    };

    struct CameraComponent
    {
        Camera camera;
        bool primary = true;

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> camera >> primary;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << camera << primary;
        }
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