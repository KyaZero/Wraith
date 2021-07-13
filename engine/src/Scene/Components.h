#pragma once

#include "Core/Math/Vec.h"
#include "Core/StringID.h"
#include "Graphics/Camera.h"
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
        StringID texture;
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

    struct TextComponent
    {
        std::string text = "hejsan kompis";
        std::string font = "assets/engine/fonts/Roboto-Regular.ttf";
        StringID font_id = StringID(font);

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> text >> font;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << text << font;
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

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer.Read(reinterpret_cast<char*>(&InstantiateScript), sizeof(InstantiateScript));
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer.Write(reinterpret_cast<const char*>(&InstantiateScript), sizeof(InstantiateScript));
        }
    };
}  // namespace fw