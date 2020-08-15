#include "VideoMode.h"
#include <Windows.h>

namespace fw
{
	VideoMode::VideoMode()
		: width(0), height(0), bpp(0)
	{
	}

	VideoMode::VideoMode(u32 mode_width, u32 mode_height, u32 mode_bpp)
		: width(mode_width), height(mode_height), bpp(mode_bpp)
	{
	}

	VideoMode VideoMode::GetDesktopMode()
	{
		DEVMODE win32_mode;
		win32_mode.dmSize = sizeof(win32_mode);
		win32_mode.dmDriverExtra = 0;
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &win32_mode);

		return VideoMode(win32_mode.dmPelsWidth, win32_mode.dmPelsHeight, win32_mode.dmBitsPerPel);
	}

	const std::vector<VideoMode> VideoMode::GetFullscreenModes()
	{
		std::vector<VideoMode> modes;

		DEVMODE win32_mode;
		win32_mode.dmSize = sizeof(win32_mode);
		win32_mode.dmDriverExtra = 0;
		for (i32 count = 0; EnumDisplaySettings(NULL, count, &win32_mode); ++count)
		{
			VideoMode mode(win32_mode.dmPelsWidth, win32_mode.dmPelsHeight, win32_mode.dmBitsPerPel);

			if (std::find(modes.begin(), modes.end(), mode) == modes.end())
				modes.push_back(mode);
		}

		return modes;
	}

	bool operator==(const VideoMode& left, const VideoMode& right)
	{
		return	(left.width == right.width) &&
			(left.height == right.height) &&
			(left.bpp == right.bpp);
	}

	bool operator!=(const VideoMode& left, const VideoMode& right)
	{
		return !(left == right);
	}

	bool operator<(const VideoMode& left, const VideoMode& right)
	{
		if (left.bpp == right.bpp)
		{
			if (left.width == right.width)
			{
				return left.height < right.height;
			}
			else
			{
				return left.width < right.width;
			}
		}
		else
		{
			return left.bpp < right.bpp;
		}
	}

	bool operator>(const VideoMode& left, const VideoMode& right)
	{
		return right < left;
	}

	bool operator<=(const VideoMode& left, const VideoMode& right)
	{
		return !(right < left);
	}

	bool operator>=(const VideoMode& left, const VideoMode& right)
	{
		return !(left < right);
	}
}