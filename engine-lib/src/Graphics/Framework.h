#pragma once
#include "Window\Window.h"
#include "Core\Math\Vec4.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace fw
{
	class Framework
	{
	public:
		Framework();
		~Framework();

		bool Init(Window* window);

		void BeginFrame(const Vec4f& clear_color);
		void EndFrame();

		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetContext();

	private:
		void ResizeBackbuffer();

		Window* m_Window;
		struct Data;
		Data* m_Data;
	};
}