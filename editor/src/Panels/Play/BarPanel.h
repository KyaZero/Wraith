#pragma once

#include "Input/Input.h"
#include "Panels/Panel.h"

namespace Wraith
{
    class Texture;
    class Editor;
    class BarPanel : public Panel
    {
        using Handler = std::function<void()>;

    public:
        BarPanel(Handler on_begin_play, Handler on_end_play, Editor& editor)
            : m_OnBeginPlay(on_begin_play)
            , m_OnEndPlay(on_end_play)
            , m_Editor(editor)
            , m_PlayButton("assets/engine/textures/editor/play_button.png")
            , m_PauseButton("assets/engine/textures/editor/pause_button.png")
            , m_TranslateButton("assets/engine/textures/editor/translate_button.png")
            , m_RotateButton("assets/engine/textures/editor/rotate_button.png")
            , m_ScaleButton("assets/engine/textures/editor/scale_button.png")
        { }

        virtual ~BarPanel() = default;

        void OnUIRender() override;
        const char* GetName() const override { return "Bar"; }

    private:
        StringID m_PlayButton;
        StringID m_PauseButton;
        StringID m_TranslateButton;
        StringID m_RotateButton;
        StringID m_ScaleButton;

        bool m_IsPlay = true;
        Handler m_OnBeginPlay;
        Handler m_OnEndPlay;
        Input m_Input;
        Editor& m_Editor;
    };
}  // namespace Wraith