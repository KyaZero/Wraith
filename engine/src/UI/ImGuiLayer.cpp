#include "ImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/imgui.h>

namespace Wraith
{
    const static auto INI_FILE_PATH = TEMP_DIRECTORY / "imgui.ini";
    const static auto INI_FILE_STRING = INI_FILE_PATH.generic_string();

    ImguiLayer::ImguiLayer(Framework& framework, Window& window)
        : m_Framework(framework)
        , m_Window(window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        ImGui_ImplWin32_Init(m_Window.GetPlatformHandle());
        ImGui_ImplDX11_Init(&m_Framework.GetDevice(), &m_Framework.GetContext());

        if (!std::filesystem::exists(INI_FILE_PATH))
        {
            std::filesystem::create_directories(INI_FILE_PATH.parent_path());
            if (std::filesystem::exists("imgui.ini"))
                std::filesystem::copy("imgui.ini", INI_FILE_PATH);
        }
        io.IniFilename = INI_FILE_STRING.c_str();

        SetStyle();
        OnContentScale(window.GetContentScale().x);

        Window::RegisterContentScaleCallback(this, [this](f32 x, f32) { OnContentScale(x); });
    }

    ImguiLayer::~ImguiLayer()
    {
        Window::UnregisterContentScaleCallback(this);
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImguiLayer::Begin()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImguiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((f32)m_Window.GetSize().x, (f32)m_Window.GetSize().x);

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);
        }

        ImGui::EndFrame();

        if (m_SettingsToLoad)
        {
            auto str = m_SettingsToLoad->generic_string();
            ImGui::LoadIniSettingsFromDisk(str.c_str());
            m_SettingsToLoad = std::nullopt;
        }
    }

    void ImguiLayer::LoadSettings(std::filesystem::path settings_file) { m_SettingsToLoad = settings_file; }

    void ImguiLayer::OnContentScale(f32 scale)
    {
        ImGui::GetStyle() = m_Style;

        auto& style = ImGui::GetStyle();
        auto& io = ImGui::GetIO();
        style.ScaleAllSizes(scale);

        // Find the best font size for the content scale
        io.FontDefault = nullptr;
        io.FontGlobalScale = 1.f;
        for (const auto& font : m_Fonts)
        {
            io.FontDefault = font.font;
            io.FontGlobalScale = scale / font.scale;

            if (font.scale >= scale)
                break;
        }
    }

    void ImguiLayer::SetStyle()
    {
        ImGui::StyleColorsDark(&m_Style);

        auto& io = ImGui::GetIO();
        auto& colors = m_Style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            m_Style.WindowRounding = 0.0f;

        for (f32 f = 1.0f; f <= 3.0f; ++f)
        {
            m_Fonts.push_back(FontData{
                .scale = f,
                .font = io.Fonts->AddFontFromFileTTF("assets/engine/fonts/Roboto-Regular.ttf", 15.f * f),
            });
        }

        ImGui::GetStyle() = m_Style;
    }
}  // namespace Wraith