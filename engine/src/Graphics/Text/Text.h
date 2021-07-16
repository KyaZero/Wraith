#pragma once

namespace fw
{
    namespace Justification
    {
        constexpr f32 Left = 0.0f;
        constexpr f32 Center = 0.5f;
        constexpr f32 Right = 1.0f;
        constexpr auto FromIndex(auto index)
        {
            return Clamp(index / 2.0f, 0.0f, 1.0f);
        }
    }  // namespace Justification
    namespace Alignment
    {
        constexpr f32 Top = 0.0f;
        constexpr f32 Middle = 0.5f;
        constexpr f32 Bottom = 1.0f;
        constexpr auto FromIndex(auto index)
        {
            return Clamp(index / 2.0f, 0.0f, 1.0f);
        }
    }  // namespace Alignment

}  // namespace fw
