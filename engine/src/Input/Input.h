#pragma once

#include "Keys.h"

struct GLFWwindow;

namespace fw
{
    class Window;
    class Input
    {
    public:
        // By default it monitors all the keys, useful for debugging etc.
        // but is slower due to having to check all keys when polling.
        Input();

        // Pass in vectors of keys you wish to monitor, useful for having
        // UI Keys and being able to handle them separately for every Input instance
        Input(std::vector<Key> keys, std::vector<MouseButton> buttons);

        ~Input();

        bool IsDown(Key key);
        bool IsDown(MouseButton button);

        bool IsUp(Key key);
        bool IsUp(MouseButton button);

        bool IsPressed(Key key);
        bool IsPressed(MouseButton button);

        bool IsReleased(Key key);
        bool IsReleased(MouseButton button);

        bool IsScrolling() const;
        ScrollState GetScrollState() const;

        bool IsEnabled();
        void SetEnabled(bool value);

        static void SetupInputs(Window* window);
        static void FlushState();
        static void BlockUIEvents(bool should_block);

    private:
        static void MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
        static void ScrollCallback(GLFWwindow* window, f64 x_offset, f64 y_offset);
        static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
        static std::vector<Input*> s_Instances;

    private:
        void SetKeyState(Key key, bool is_down);
        void SetMouseButtonState(MouseButton button, bool is_down);
        void SetScrollState(f64 x_offset, f64 y_offset);

        std::unordered_map<i32, bool> m_Keys;
        std::unordered_map<i32, bool> m_KeysPrevious;
        std::unordered_map<i32, bool> m_MouseButtons;
        std::unordered_map<i32, bool> m_MouseButtonsPrevious;
        ScrollState m_ScrollState;

        bool m_IsEnabled;
        static bool s_UIBlockEvents;
    };
}  // namespace fw