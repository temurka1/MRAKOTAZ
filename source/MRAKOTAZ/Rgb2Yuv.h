#ifndef RBG_2_YUV_H
#define RBG_2_YUV_H

namespace MRAKOTAZ::core::conversions
{
	/// <summary>
	/// Converts rgb to YUV 4:2:0 with coefficients from BT.601
	/// VU consists of interleaved V and U values
	/// </summary>
	void rgb2yuv(const std::vector<uint8_t>& rgb, const uint32_t width, const uint32_t height, uint8_t* y, uint8_t* vu);
}

#endif
