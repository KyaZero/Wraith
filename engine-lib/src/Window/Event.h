#pragma once
#include "..\Core\Types.h"
#include "Keys.h"
#include "Mouse.h"

namespace fw
{
	struct Event
	{
		enum EventType
		{
			Closed,
			Resized,
			GainedFocus,
			LostFocus,

			KeyPressed,
			KeyReleased,

			MouseLeave,
			MouseEnter,
			MouseMove,
			MouseWheelMoved,
			MouseWheelScrolled,
			MouseButtonPressed,
			MouseButtonReleased,
		} type;

		struct MouseMoveEvent
		{
			i32 x;
			i32 y;
		};

		struct MouseWheelEvent
		{
			i32 delta;
			i32 x;
			i32 y;
		};

		struct MouseWheelScrollEvent
		{
			Mouse::Wheel wheel;
			f32 delta;
			i32 x;
			i32 y;
		};

		struct MouseButtonEvent
		{
			Mouse::Button button;
			i32 x;
			i32 y;
		};

		struct SizeEvent
		{
			u32 width;
			u32 height;
		};

		struct KeyEvent
		{
			Key code;
			bool alt;
			bool control;
			bool shift;
			bool system;
		};

		union
		{
			SizeEvent size;
			KeyEvent key;
			MouseMoveEvent mouse_move;
			MouseWheelEvent mousewheel;
			MouseWheelScrollEvent mousewheel_scroll;
			MouseButtonEvent mouse_button;
		};
	};
}