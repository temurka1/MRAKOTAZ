#include "pch.h"
#include "BitmapFrame.h"

using BitmapFrame = MRAKOTAZ::bitmap::BitmapFrame;

BitmapFrame::BitmapFrame(std::vector<uint8_t>& data, uint32_t width, uint32_t height, uint32_t bpp) :
	_data(std::move(data)), _width(width), _height(height), _bpp(bpp)
{
}

uint32_t BitmapFrame::Width() const
{
	return _width;
}

uint32_t BitmapFrame::Height() const
{
	return _height;
}

uint32_t BitmapFrame::Bpp() const
{
	return _bpp;
}

const std::vector<uint8_t>& BitmapFrame::Data() const
{
	return _data;
}