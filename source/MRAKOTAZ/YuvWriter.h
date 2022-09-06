#ifndef YUV_WRITER_H
#define YUV_WRITER_H

#include <fstream>
#include "Dimension.h"

namespace MRAKOTAZ::yuv
{
	namespace core = MRAKOTAZ::core;

	struct YuvFrame;

	/// <summary>
	/// Writer for YUV format
	/// </summary>
	class YuvWriter
	{
	public:
		YuvWriter(const std::string& filename, const core::Dimension& dimension);
		~YuvWriter();

		/// <summary>
		/// writes {framesCount} frames from given vector into output file 
		/// </summary>
		void WriteFrames(std::vector<YuvFrame>& frames, size_t framesCount);
	private:
		std::ofstream _fileStream;
		core::Dimension _dimension;
	};
}

#endif
