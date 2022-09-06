#include "pch.h"
#include "OverlayBuilder.h"

#include "BitmapFrame.h"

#include "YuvReader.h"
#include "YuvWriter.h"

#include "ResultOr.hpp"

#include "Dimension.h"
#include "Rgb2Yuv.h"

using OverlayBuilder = MRAKOTAZ::overlay::OverlayBuilder;

using BitmapFrame = MRAKOTAZ::bitmap::BitmapFrame;

using YuvFrame = MRAKOTAZ::yuv::YuvFrame;
using YuvReader = MRAKOTAZ::yuv::YuvReader;
using YuvWriter = MRAKOTAZ::yuv::YuvWriter;

using Dimension = MRAKOTAZ::core::Dimension;

namespace core = MRAKOTAZ::core;
namespace jobs = MRAKOTAZ::core::jobs;
namespace conversions = MRAKOTAZ::core::conversions;

namespace yuv = MRAKOTAZ::yuv;

namespace
{
	YuvFrame convert_to_yuv(const BitmapFrame& rgb)
	{
		YuvFrame converted;
		converted.width = rgb.Width();
		converted.height = rgb.Height();

		converted.y_plane = new uint8_t[converted.width * converted.height];
		converted.vu_plane = new uint8_t[converted.width * converted.height];

		conversions::rgb2yuv(rgb.Data(), converted.width, converted.height, converted.y_plane, converted.vu_plane);

		return converted;
	}
}

OverlayBuilder::OverlayBuilder(const BitmapFrame& rgb, const Dimension& dim, const std::string& yuvIn, const std::string& yuvOut) : 
	_reader(std::make_unique<YuvReader>(yuvIn, dim)), 
	_writer(std::make_unique<YuvWriter>(yuvOut, dim)), 
	_yuvOverlay(convert_to_yuv(rgb))
{
	_jobManager = std::make_unique<jobs::JobManager<YuvFrame>>([this](YuvFrame& frame)
	{
		yuv::blend_frames(frame, _yuvOverlay, 1.0);
	});
}

OverlayBuilder::~OverlayBuilder()
{
	delete[] _yuvOverlay.y_plane;
	delete[] _yuvOverlay.vu_plane;
}

core::Result OverlayBuilder::Build()
{
	// count of frames to simultaneously process
	constexpr uint32_t BUFFERING_FRAMES_COUNT = 16;

	// Allocate reusable buffer for BUFFERING_FRAMES_COUNT frames.
	std::vector<YuvFrame> buffer(BUFFERING_FRAMES_COUNT);

	uint8_t collectedFramesCount = 0;

	while (true)
	{
		core::Result status = _reader->ReadFrame(&buffer[collectedFramesCount++]);
		if (status.Code() == core::Result::ERROR)
		{
			return status;
		}

		bool isEof = status.Code() == core::Result::NOT_OK;

		// we collected enough frames to process or reached eof
		//
		if (collectedFramesCount == BUFFERING_FRAMES_COUNT || isEof)
		{
			for (size_t i = 0; i < collectedFramesCount; ++i)
			{
				// push collected frames to job manager
				_jobManager->Enqueue(buffer[i]);
			}

			// wait for pending jobs to complete
			_jobManager->WaitForAll();

			// write processed data to file
			_writer->WriteFrames(buffer, collectedFramesCount);

			// cleanup
			collectedFramesCount = 0;
		}

		if (isEof)
		{
			break;
		}
	}

	return core::Result(core::Result::OK);
}