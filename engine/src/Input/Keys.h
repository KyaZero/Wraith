#pragma once

namespace Wraith
{
    struct MouseState
    {
        f64 x_pos;
        f64 y_pos;
    };

    struct ScrollState
    {
        f64 x_offset;
        f64 y_offset;
    };

    enum class MouseButton
    {
        Left,
        Right,
        Middle,
        Count
    };

    enum class Key
    {
        BACKSPACE = 0x08,
        TAB = 0x09,
        RETURN = 0x0D,
        SHIFT = 0x10,
        CONTROL = 0x11,
        MENU = 0x12,
        PAUSE = 0x13,
        CAPSLOCK = 0x14,
        ESCAPE = 0x1B,
        SPACEBAR = 0x20,
        PGUP = 0x21,
        PGDN = 0x22,
        END = 0x23,
        HOME = 0x24,
        LEFT = 0x25,
        UP = 0x26,
        RIGHT = 0x27,
        DOWN = 0x28,
        SNAPSHOT = 0x2C,
        INSERT = 0x2D,
        DEL = 0x2E,
        KB0 = 0x30,
        KB1 = 0x31,
        KB2 = 0x32,
        KB3 = 0x33,
        KB4 = 0x34,
        KB5 = 0x35,
        KB6 = 0x36,
        KB7 = 0x37,
        KB8 = 0x38,
        KB9 = 0x39,
        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4A,
        K = 0x4B,
        L = 0x4C,
        M = 0x4D,
        N = 0x4E,
        O = 0x4F,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5A,
        LWIN = 0x5B,
        RWIN = 0x5C,
        NUMPAD0 = 0x60,
        NUMPAD1 = 0x61,
        NUMPAD2 = 0x62,
        NUMPAD3 = 0x63,
        NUMPAD4 = 0x64,
        NUMPAD5 = 0x65,
        NUMPAD6 = 0x66,
        NUMPAD7 = 0x67,
        NUMPAD8 = 0x68,
        NUMPAD9 = 0x69,
        MULTIPLY = 0x6A,
        ADD = 0x6B,
        SUBTRACT = 0x6D,
        DECIMAL = 0x6E,
        DIVIDE = 0x6F,
        F1 = 0x70,
        F2 = 0x71,
        F3 = 0x72,
        F4 = 0x73,
        F5 = 0x74,
        F6 = 0x75,
        F7 = 0x76,
        F8 = 0x77,
        F9 = 0x78,
        F10 = 0x79,
        F11 = 0x7A,
        F12 = 0x7B,

        LSHIFT = 340,
        LCONTROL = 341,
        LALT = 342,
        Count
    };
}  // namespace Wraith