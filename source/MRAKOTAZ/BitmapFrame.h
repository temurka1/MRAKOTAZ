#ifndef BITMAP_IMAGE_H
#define BITMAP_IMAGE_H

#include <vector>

namespace MRAKOTAZ::bitmap
{
	class BitmapFrame
	{
	public:
		BitmapFrame() = default;
		BitmapFrame(std::vector<uint8_t>& data, uint32_t width, uint32_t height, uint32_t bpp);

		/// <summary>
		/// Returns frame's width
		/// </summary>
		uint32_t Width() const;

		/// <summary>
		/// Returns frame's height
		/// </summary>
		uint32_t Height() const;

		/// <summary>
		/// Returns bits per pixel
		/// </summary>
		uint32_t Bpp() const;

		/// <summary>
		/// Returns raw bitmap data
		/// </summary>
		const std::vector<uint8_t>& Data() const;
	private:
		uint32_t _width;
		uint32_t _height;

		uint32_t _bpp;

		// rgb = (data[0], data[1], data[2])
		std::vector<uint8_t> _data;
	};
}

#endif

