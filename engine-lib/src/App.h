#pragma once
#include "Window/Window.h"
#include "Graphics/Framework.h"
#include "Graphics/RenderManager.h"
#include "Core/Timer.h"
#include <queue>

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
		Framework m_Framework;
		RenderManager m_RenderManager;
		Timer m_Timer;

		//for averaging out fps
		std::deque<f32> m_LastTimes;
		constexpr static i32 MaxNumTimesSaved = 60;
	};
}