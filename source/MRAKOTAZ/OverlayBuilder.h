#ifndef OVERLAY_BUILDER_H
#define OVERLAY_BUILDER_H

#include "YuvFrame.h"
#include "JobManager.hpp"

namespace MRAKOTAZ::core
{
	class Result;
	class Dimension;
}

namespace MRAKOTAZ::yuv
{
	class YuvReader;
	class YuvWriter;
}

namespace MRAKOTAZ::bitmap
{
	class BitmapFrame;
}

namespace MRAKOTAZ::overlay
{
	namespace core = MRAKOTAZ::core;
	namespace jobs = MRAKOTAZ::core::jobs;

	namespace yuv = MRAKOTAZ::yuv;
	namespace bitmap = MRAKOTAZ::bitmap;

	/// <summary>
	/// Applies given bitmap overlay over range of YUV video frames and dumps result into output file
	/// </summary>
	class OverlayBuilder
	{
	public:
		OverlayBuilder(const bitmap::BitmapFrame& rgb, const core::Dimension& dim, const std::string& yuvIn, const std::string& yuvOut);
		~OverlayBuilder();

		core::Result Build();
	private:
		std::unique_ptr<jobs::JobManager<yuv::YuvFrame>> _jobManager;

		yuv::YuvFrame _yuvOverlay;

		std::unique_ptr<yuv::YuvWriter> _writer;
		std::unique_ptr<yuv::YuvReader> _reader;
	};
}

#endif