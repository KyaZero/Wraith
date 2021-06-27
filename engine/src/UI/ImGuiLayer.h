#pragma once

#include <memory>

#include "Engine.h"

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
}  // namespace fw