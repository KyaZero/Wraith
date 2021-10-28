#pragma once

namespace Wraith
{
    namespace Justification
    {
        constexpr i32 Left = 0;
        constexpr i32 Center = 1;
        constexpr i32 Right = 2;
        constexpr auto FromIndex(auto index)
        {
            return Clamp(index / 2.0f, 0.0f, 1.0f);
        }
    }  // namespace Justification
    namespace Alignment
    {
        constexpr i32 Top = 0;
        constexpr i32 Middle = 1;
        constexpr i32 Bottom = 2;
        constexpr auto FromIndex(auto index)
        {
            return Clamp(index / 2.0f, 0.0f, 1.0f);
        }
    }  // namespace Alignment
    namespace Direction
    {
        constexpr i32 LTR = 0;
        constexpr i32 RTL = 1;
    }  // namespace Direction

}  // namespace fw
