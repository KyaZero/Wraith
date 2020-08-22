#pragma once
#include "../Core/Types.h"
#include <vector>

namespace fw
{
	class VideoMode
	{
	public:
		VideoMode();
		VideoMode(u32 mode_width, u32 mode_height, u32 mode_bpp = 32);

		static VideoMode GetDesktopMode();
		static const std::vector<VideoMode> GetFullscreenModes();

		u32 width;
		u32 height;
		u32 bpp;

	};

	bool operator ==(const VideoMode& left, const VideoMode& right);
	bool operator !=(const VideoMode& left, const VideoMode& right);
	bool operator <(const VideoMode& left, const VideoMode& right);
	bool operator >(const VideoMode& left, const VideoMode& right);
	bool operator <=(const VideoMode& left, const VideoMode& right);
	bool operator >=(const VideoMode& left, const VideoMode& right);
}