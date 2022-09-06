#include "pch.h"
#include "Rgb2Yuv.h"

namespace
{
	inline uint8_t clamp_255(int32_t v)
	{
		return (uint8_t)((v < 0) ? 0 : ((v > 255) ? 255 : v));
	}
}

namespace MRAKOTAZ::core::conversions
{
	void rgb2yuv(const std::vector<uint8_t>& rgb, const uint32_t width, const uint32_t height, uint8_t* y, uint8_t* vu)
	{
		assert(y != nullptr);
		assert(vu != nullptr);

		int index = 0, yIndex = 0, vuIndex = 0;

		for (uint32_t j = 0; j < height; j++)
		{
			for (uint32_t i = 0; i < width; i++)
			{
				uint8_t R = rgb[index++];
				uint8_t G = rgb[index++];
				uint8_t B = rgb[index++];

				int32_t Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
				int32_t U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
				int32_t V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

				y[yIndex++] = clamp_255(Y);

				if (j % 2 == 0 && index % 2 == 0)
				{
					// TODO: use bilinear filtering?

					vu[vuIndex++] = clamp_255(V);
					vu[vuIndex++] = clamp_255(U);
				}
			}
		}
	}
}