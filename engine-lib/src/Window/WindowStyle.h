#pragma once

namespace fw
{
	namespace Style
	{
		enum
		{
			None = 0,
			Titlebar = 1 << 0,
			Resize = 1 << 1,
			Close = 1 << 2,
			Fullscreen = 1 << 3,
			Minimize = 1 << 4,

			Default = Titlebar | Resize | Close | Minimize
		};
	}
}