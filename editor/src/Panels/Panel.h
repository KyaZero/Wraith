#pragma once

namespace Wraith
{
    class Panel
    {
    public:
        Panel() = default;
        virtual ~Panel() = default;

        virtual void OnUIRender() = 0;
        virtual const char* GetName() const = 0;

        void ToggleVisibility()
        {
            m_IsVisible = !m_IsVisible;
        }

        bool IsVisible() const
        {
            return m_IsVisible;
        }
        bool IsFocused() const
        {
            return m_IsFocused;
        }
        bool IsHovered() const
        {
            return m_IsHovered;
        }

    protected:
        bool Begin(int flags = 0);
        void End();

        bool m_IsVisible = true;
        bool m_IsFocused = false;
        bool m_IsHovered = false;

    private:
        bool m_ShouldCallEnd = true;
    };
}  // namespace Wraith