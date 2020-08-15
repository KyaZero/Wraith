#pragma once
#include "Window/Window.h"
#include "Graphics/Framework.h"

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
	};
}