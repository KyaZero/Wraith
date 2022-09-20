#pragma once

#include "Core/Math/Vec.h"
#include "Core/StringID.h"
#include "Graphics/Camera.h"
#include "Graphics/Text/Text.h"
#include "Graphics/VkModel.h"
#include "ScriptableEntity.h"

namespace Wraith
{
    struct TransformComponent
    {
        Vec3f position{ 0.f, 0.f, 0.f };
        Quatf rotation{ 1, 0, 0, 0 };
        Vec3f scale{ 1.f, 1.f, 1.f };

        Mat4f GetTransform() const { return Mat4f::CreateTransform(position, rotation, scale); }

        void Serialize(dubu::serialize::ReadBuffer& buffer) { buffer >> position >> scale >> rotation; }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const { buffer << position << scale << rotation; }
    };

    struct TagComponent
    {
        std::string tag;

        void Serialize(dubu::serialize::ReadBuffer& buffer) { buffer >> tag; }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const { buffer << tag; }
    };

    struct SpriteComponent
    {
        StringID texture;
        Vec4f color{ 1, 1, 1, 1 };
        Vec2f origin{ 0, 0 };
        f32 layer{ 0 };
        bool screen_space{ false };

        void Serialize(dubu::serialize::ReadBuffer& buffer) { buffer >> texture >> color >> origin >> layer >> screen_space; }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const { buffer << texture << color << origin << layer << screen_space; }
    };

    struct TextComponent
    {
        std::string text = "Text";
        std::string font = "assets/engine/fonts/roboto-regular.ttf";
        StringID font_id = font;
        i32 justification = Justification::Left;
        i32 alignment = Alignment::Top;
        i32 font_size = 36;
        i32 direction = Direction::LTR;
        Vec4f color{ 1, 1, 1, 1 };
        f32 blend_mode = 1.f;

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> text >> font >> justification >> alignment >> font_size >> direction >> color >> blend_mode;
            font_id = font;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << text << font << justification << alignment << font_size << direction << color << blend_mode;
        }
    };

    struct CameraComponent
    {
        Camera camera;
        bool primary = true;

        void Serialize(dubu::serialize::ReadBuffer& buffer) { buffer >> camera >> primary; }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const { buffer << camera << primary; }
    };

    struct ModelComponent
    {
        // Model model_instance;
        std::string model_name;
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

        void DestroyScript() { instance.reset(); }

        void Serialize(dubu::serialize::ReadBuffer& buffer) { buffer.Read(reinterpret_cast<char*>(&InstantiateScript), sizeof(InstantiateScript)); }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer.Write(reinterpret_cast<const char*>(&InstantiateScript), sizeof(InstantiateScript));
        }
    };
}  // namespace Wraith