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
            mIsVisible = !mIsVisible;
        }

        bool IsVisible() const
        {
            return mIsVisible;
        }
        bool IsFocused() const
        {
            return mIsFocused;
        }
        bool IsHovered() const
        {
            return mIsHovered;
        }

    protected:
        bool Begin(int flags = 0);
        void End();

        bool mIsVisible = true;
        bool mIsFocused = false;
        bool mIsHovered = false;

    private:
        bool mShouldCallEnd = true;
    };
}  // namespace Wraith