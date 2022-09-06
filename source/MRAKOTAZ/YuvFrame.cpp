#include "pch.h"
#include "YuvFrame.h"
#include "Blending.h"

using YuvFrame = MRAKOTAZ::yuv::YuvFrame;

namespace conversions = MRAKOTAZ::core::conversions;

namespace
{
	void blend_frame_planes(uint8_t* b, uint8_t* f, const uint32_t bW, const uint32_t fW, const uint32_t fH, const float c)
	{
		for (uint32_t i = 0; i < fW; ++i)
		{
			for (uint32_t j = 0; j < fH; ++j)
			{
				const uint32_t bIndex = bW * i + j;
				const uint32_t fIndex = fW * i + j;

				b[bIndex] = conversions::blending_alpha(b[bIndex], f[fIndex], c);
			}
		}
	}
}

namespace MRAKOTAZ::yuv
{
	void blend_frames(YuvFrame& background, const YuvFrame& foreground, const float blendingCoeff)
	{
		assert(background.height <= foreground.height);
		assert(background.width <= foreground.width);

		blend_frame_planes(background.y_plane, foreground.y_plane, background.width, foreground.width, foreground.height, blendingCoeff);
		blend_frame_planes(background.vu_plane, foreground.vu_plane, background.width, foreground.width, foreground.height, blendingCoeff);
	}
}