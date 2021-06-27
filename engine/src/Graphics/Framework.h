#pragma once

#include "Core/Math/Vec4.h"
#include "Texture.h"
#include "Window/Window.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace fw
{
	class Framework
	{
	public:
		Framework();
		~Framework();

		bool Init(std::shared_ptr<Window> window);

		void BeginFrame(const Vec4f& clear_color);
		void EndFrame();

		void SetBackbufferAsActiveTarget();

		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetContext();

		static void BeginEvent(std::string name);
		static void EndEvent();

	private:
		void ResizeBackbuffer();

		std::shared_ptr<Window> m_Window;
		struct Data;
		Data* m_Data;
	};
}  // namespace fw