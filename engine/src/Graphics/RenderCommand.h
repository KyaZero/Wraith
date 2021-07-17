#pragma once

#include <variant>

#include "Camera.h"
#include "Core/StringID.h"

namespace fw
{
    struct SpriteCommand
    {
        StringID texture;
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

    struct TextCommand
    {
        std::string text;
        StringID font_id;
        f32 justification;
        f32 alignment;
        i32 font_size;
        i32 direction;
        Vec4f color;
        f32 blend_mode;
    };

    using RenderCommand = std::variant<SpriteCommand, SetCameraCommand, TextCommand>;
}  // namespace fw