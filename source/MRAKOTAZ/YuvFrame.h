#ifndef YUV_FRAME_H
#define YUV_FRAME_H

namespace MRAKOTAZ::yuv
{
	/// <summary>
	/// YUV frame
	/// YUV frame has to be trivially copiable to work with JobManager/JobQueue.
	/// </summary>
	struct YuvFrame
	{
		uint32_t width;
		uint32_t height;

		uint8_t* y_plane;
		uint8_t* vu_plane;
	};

	/// <summary>
	/// Blends foreground YUV frame over background YUV frame
	/// </summary>
	void blend_frames(YuvFrame& background, const YuvFrame& foreground, const float blendingCoeff);
}

#endif