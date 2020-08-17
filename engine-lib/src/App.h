#pragma once
#include "Window/Window.h"
#include "Core/Timer.h"
#include "Engine.h"

namespace fw
{
	class App
	{
	public:
		App();
		~App();

		bool Start();
		bool Run();

	private:
		Window* m_Window;
		Timer m_Timer;
		Engine m_Engine;
	};
}