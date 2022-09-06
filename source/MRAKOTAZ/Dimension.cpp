#include "pch.h"
#include "Dimension.h"

#include "ResultOr.hpp"

namespace MRAKOTAZ::core
{
    const static std::unordered_map<std::string, Dimension> static_dimensions
    {
        { "qcif", { 176,  144} },
        { "sif", { 352,  240} },
        { "cif", { 352,  288} },
        { "vga", { 640,  480} },
        { "ntsc", { 700,  525} },
        { "sd576", { 720,  576} },
        { "pal", { 780,  586} },
        { "hd720", { 1280,  720} },
        { "hd1080", { 1920,  1080} },
    };

	ResultOr<Dimension> Dimension::FromString(const std::string& str)
	{
        auto search = static_dimensions.find(str);

        if (search != static_dimensions.end())
        {
            return ResultOr<Dimension>(search->second);
        }

        // not supported dimension
        return ResultOr<Dimension>(Result(Result::ERROR));
	}
}