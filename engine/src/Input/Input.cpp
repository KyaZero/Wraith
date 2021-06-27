#include "Input.h"
#include <Window/Window.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

namespace fw
{
	std::vector<Input*> Input::s_Instances;
	bool Input::s_UIBlockEvents = false;

	Input::Input() : m_IsEnabled(true), m_ScrollState{ 0, 0 }
	{
		for (i32 i = 0; i < (i32)Key::Count; ++i)
		{
			m_Keys[i] = false;
		}

		for (i32 i = 0; i < (i32)MouseButton::Count; ++i)
		{
			m_MouseButtons[i] = false;
		}

		s_Instances.push_back(this);
	}

	Input::Input(std::vector<Key> keys, std::vector<MouseButton> buttons) : m_IsEnabled(true), m_ScrollState{ 0, 0 }
	{
		for (Key key : keys)
		{
			m_Keys[(i32)key] = false;
			m_KeysPrevious[(i32)key] = false;
		}

		for (MouseButton button : buttons)
		{
			m_MouseButtons[(i32)button] = false;
			m_MouseButtonsPrevious[(i32)button] = false;
		}

		s_Instances.push_back(this);
	}

	Input::~Input()
	{
		s_Instances.erase(std::remove(s_Instances.begin(), s_Instances.end(), this), s_Instances.end());
	}

	bool Input::IsDown(Key key)
	{
		bool result = false;

		if (m_IsEnabled)
		{
			auto it = m_Keys.find((i32)key);
			if (it != m_Keys.end())
			{
				result = m_Keys[(i32)key];
			}
		}

		return result;
	}

	bool Input::IsDown(MouseButton button)
	{
		bool result = false;

		if (m_IsEnabled)
		{
			auto it = m_MouseButtons.find((i32)button);
			if (it != m_MouseButtons.end())
			{
				result = m_MouseButtons[(i32)button];
			}
		}

		return result;
	}

	bool Input::IsUp(Key key)
	{
		return !IsDown(key);
	}

	bool Input::IsUp(MouseButton button)
	{
		return !IsDown(button);
	}

	bool Input::IsPressed(Key key)
	{
		bool result = false;

		if (m_IsEnabled)
		{
			auto it = m_Keys.find((i32)key);
			if (it != m_Keys.end())
			{
				result = (m_Keys[(i32)key] && !m_KeysPrevious[(i32)key]);
			}
		}

		return result;
	}

	bool Input::IsPressed(MouseButton button)
	{
		bool result = false;

		if (m_IsEnabled)
		{
			auto it = m_MouseButtons.find((i32)button);
			if (it != m_MouseButtons.end())
			{
				result = (m_MouseButtons[(i32)button] && !m_MouseButtonsPrevious[(i32)button]);
			}
		}

		return result;
	}

	bool Input::IsReleased(Key key)
	{
		bool result = false;

		if (m_IsEnabled)
		{
			auto it = m_Keys.find((i32)key);
			if (it != m_Keys.end())
			{
				result = (!m_Keys[(i32)key] && m_KeysPrevious[(i32)key]);
			}
		}

		return result;
	}

	bool Input::IsReleased(MouseButton button)
	{
		bool result = false;

		if (m_IsEnabled)
		{
			auto it = m_MouseButtons.find((i32)button);
			if (it != m_MouseButtons.end())
			{
				result = (!m_MouseButtons[(i32)button] && m_MouseButtonsPrevious[(i32)button]);
			}
		}

		return result;
	}

	bool Input::IsScrolling() const
	{
		return m_ScrollState.x_offset != 0 || m_ScrollState.y_offset != 0;
	}

	ScrollState Input::GetScrollState() const
	{
		return m_ScrollState;
	}

	void Input::SetKeyState(Key key, bool is_down)
	{
		auto it = m_Keys.find((i32)key);
		if (it != m_Keys.end())
		{
			m_Keys[(i32)key] = is_down;
		}
	}

	void Input::SetMouseButtonState(MouseButton button, bool is_down)
	{
		auto it = m_MouseButtons.find((i32)button);
		if (it != m_MouseButtons.end())
		{
			m_MouseButtons[(i32)button] = is_down;
		}
	}

	void Input::SetScrollState(f64 x_offset, f64 y_offset)
	{
		m_ScrollState = { x_offset, y_offset };
	}

	bool Input::IsEnabled()
	{
		return m_IsEnabled;
	}

	void Input::SetEnabled(bool value)
	{
		m_IsEnabled = value;
	}

	void Input::SetupInputs(Window* window)
	{
		glfwSetKeyCallback(window->GetHandle(), Input::KeyCallback);
		glfwSetMouseButtonCallback(window->GetHandle(), Input::MouseButtonCallback);
		glfwSetScrollCallback(window->GetHandle(), Input::ScrollCallback);
	}

	void Input::FlushState()
	{
		for (Input* input : s_Instances)
		{
			input->SetScrollState(0, 0);

			for (auto& kv : input->m_Keys)
			{
				input->m_KeysPrevious[kv.first] = kv.second;
			}

			for (auto& kv : input->m_MouseButtons)
			{
				input->m_MouseButtonsPrevious[kv.first] = kv.second;
			}
		}
	}

	void Input::BlockUIEvents(bool should_block)
	{
		s_UIBlockEvents = should_block;
	}

	void Input::MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods)
	{
		if (s_UIBlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse)
				return;
		}

		for (Input* input : s_Instances)
		{
			input->SetMouseButtonState((MouseButton)button, action != GLFW_RELEASE);
		}
	}

	void Input::ScrollCallback(GLFWwindow* window, f64 x_offset, f64 y_offset)
	{
		if (s_UIBlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse)
				return;
		}

		for (Input* input : s_Instances)
		{
			input->SetScrollState(x_offset, y_offset);
		}
	}

	void Input::KeyCallback([[maybe_unused]] GLFWwindow* window, i32 key, [[maybe_unused]] i32 scancode, i32 action, [[maybe_unused]] i32 mods)
	{
		if (s_UIBlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureKeyboard)
				return;
		}

		for (Input* input : s_Instances)
		{
			input->SetKeyState((Key)key, action != GLFW_RELEASE);
		}
	}
}