#ifndef BITMAP_READER_H
#define BITMAP_READER_H

namespace MRAKOTAZ
{
	namespace core
	{
		template <typename T> class ResultOr;
	}

	namespace bitmap
	{
		namespace core = MRAKOTAZ::core;
		class BitmapFrame;

		/// <summary>
		/// Reads bitmap from file
		/// </summary>
		core::ResultOr<BitmapFrame> read_bitmap_frame(const std::string& filename);
	}
}

#endif
