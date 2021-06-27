#pragma once

#include "Engine.h"
#include <memory>

namespace fw
{
	class ImguiLayer
	{
	public:
		ImguiLayer(std::shared_ptr<Engine> engine);
		~ImguiLayer();

		void Begin();
		void End();

		void SetThemeColors();

	private:
		std::shared_ptr<Engine> m_Engine;
	};
}