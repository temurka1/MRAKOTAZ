#include "pch.h"
#include "YuvWriter.h"
#include "YuvFrame.h"

using YuvFrame = MRAKOTAZ::yuv::YuvFrame;
using YuvWriter = MRAKOTAZ::yuv::YuvWriter;

using Dimension = MRAKOTAZ::core::Dimension;

namespace
{
	inline void write_array(std::ofstream& stream, uint8_t* data, const std::streamsize size)
	{
		stream.write(reinterpret_cast<char*>(data), size);
	}
}

YuvWriter::YuvWriter(const std::string& filename, const Dimension& dimension) :
	_fileStream(std::ofstream(filename.c_str(), std::ios::binary)), _dimension(dimension)
{

}

YuvWriter::~YuvWriter()
{
	if (_fileStream.is_open())
	{
		_fileStream.close();
	}
}

void YuvWriter::WriteFrames(std::vector<YuvFrame>& frames, size_t framesCount)
{
	assert(_fileStream.is_open());

	for (size_t i = 0; i < framesCount; ++i)
	{
		const YuvFrame& frame = frames[i];

		std::streamsize size = frame.width * frame.height * sizeof(uint8_t);

		// write Y plane
		write_array(_fileStream, frame.y_plane, size);

		// write UV planes
		write_array(_fileStream, frame.vu_plane, size);
	}
}
