#ifndef YUV_READ_H
#define YUV_READ_H

#include <fstream>
#include "Dimension.h"

namespace MRAKOTAZ::core
{
	class Result;
}

namespace MRAKOTAZ::yuv
{
	namespace core = MRAKOTAZ::core;

	struct YuvFrame;

	/// <summary>
	/// Reads YUV frames from file
	/// </summary>
	class YuvReader
	{
	public:
		YuvReader(const std::string& filename, const core::Dimension& dimension);
		~YuvReader();

		/// <summary>
		/// Reads next frame into given frame object
		/// </summary>
		core::Result ReadFrame(YuvFrame* frame);
	private:
		std::ifstream _fileStream;
		core::Dimension _dimension;
	};
}

#endif

