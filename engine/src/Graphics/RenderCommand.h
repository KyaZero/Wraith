#pragma once

#include <variant>

#include "Camera.h"
#include "Core/StringID.h"
#include "Models/Model.h"

namespace Wraith
{
    struct SpriteCommand
    {
        StringID texture;
        Vec4f color;
        Vec4f position;
        Vec2f origin;
        Vec2f scale;
        f32 rotation;
        f32 layer;
        bool screen_space;
    };

    struct SetCameraCommand
    {
        Mat4f proj;
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
        Vec2f screen_position;
    };

    struct ModelCommand
    {
        Model model;
        Mat4f transform;
    };

    using RenderCommand = std::variant<SpriteCommand, SetCameraCommand, TextCommand, ModelCommand>;
}  // namespace Wraith
