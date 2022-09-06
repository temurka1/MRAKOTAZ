// MRAKOTAZ.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <argparse/argparse.hpp>

#include "ResultOr.hpp"

#include "BitmapReader.h"
#include "BitmapFrame.h"

#include "YuvReader.h"
#include "YuvWriter.h"

#include "OverlayBuilder.h"

namespace core = MRAKOTAZ::core;
namespace bitmap = MRAKOTAZ::bitmap;

using ArgumentParser = argparse::ArgumentParser;

using BitmapFrame = bitmap::BitmapFrame;
using OverlayBuilder = MRAKOTAZ::overlay::OverlayBuilder;

// Usage: 
// MRAKOTAZ.exe --ycbcr-in video.yuv --ycbcr-out video_out.yuv --overlay-bmp overlay.bmp --dim=qcif 

int main(int argc, char* argv[])
{
    ArgumentParser program("MRAKOTAZ");

    program.add_description("RGB8 bitmap overlay over YUV420p video");

    program.add_argument("--ycbcr-in").help("YUV420p video input file").required();
    program.add_argument("--ycbcr-out").help("YUV420p video output file").required();
    program.add_argument("--overlay-bmp").help("RGB8 bitmap overlay file").required();
    program.add_argument("--dim").help("YUV420p video width").required();
    program.add_argument("help");

    program.parse_args(argc, argv);

    // read bitmap overlay
    //
    core::ResultOr<BitmapFrame> overlayFrameResult = bitmap::read_bitmap_frame(program.get<std::string>("--overlay-bmp"));
    if (!overlayFrameResult.Ok())
    {
        std::cerr << overlayFrameResult.Res().Error() << std::endl;
        return -1;
    }

    core::ResultOr<core::Dimension> dimensionsResult = core::Dimension::FromString(program.get<std::string>("--dim"));
    if (!dimensionsResult.Ok())
    {
        std::cerr << dimensionsResult.Res().Error() << std::endl;
        return -1;
    }

    const std::string yuvInput = program.get<std::string>("--ycbcr-in");
    const std::string yuvOutput = program.get<std::string>("--ycbcr-out");

    // read input YUV video, put overlay over (sic!) and write to output YUV
    OverlayBuilder overlayBuilder(overlayFrameResult.Value(), dimensionsResult.Value(), yuvInput, yuvOutput);
    overlayBuilder.Build();

    return 0;
}
