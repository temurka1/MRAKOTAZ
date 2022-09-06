#include "pch.h"
#include "YuvReader.h"
#include "YuvFrame.h"

#include "Result.hpp"

using YuvFrame = MRAKOTAZ::yuv::YuvFrame;
using YuvReader = MRAKOTAZ::yuv::YuvReader;

using Result = MRAKOTAZ::core::Result;
using Dimension = MRAKOTAZ::core::Dimension;

namespace
{
	inline void read_array(std::ifstream& stream, uint8_t* data, const std::streamsize size)
	{
		stream.read(reinterpret_cast<char*>(data), size);
	}
}

YuvReader::YuvReader(const std::string& filename, const Dimension& dimension) :
	_fileStream(std::ifstream(filename.c_str(), std::ios::binary)), _dimension(dimension)
{
}

YuvReader::~YuvReader()
{
	if (_fileStream.is_open())
	{
		_fileStream.close();
	}
}

Result YuvReader::ReadFrame(YuvFrame* frame)
{
	assert(_fileStream.is_open());
	assert(frame != nullptr);

	if (_fileStream.eof())
		return Result(Result::NOT_OK);

	const std::streamsize size = frame->width * frame->height * sizeof(uint8_t);

	// NV21 has a plane of Y and interleaved planes of VU each sampled by a factor of 2
	// NV21  YYYYYYYY VUVU

	// read Y plane
	read_array(_fileStream, frame->y_plane, size);

	// read V and U planes
	read_array(_fileStream, frame->vu_plane, size);

	return Result();
}