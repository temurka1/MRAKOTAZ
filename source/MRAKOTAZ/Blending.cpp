#include "pch.h"
#include "Blending.h"

namespace MRAKOTAZ::core::conversions
{
	uint8_t blending_alpha(const uint8_t& background, const uint8_t& foreground, const float& alpha)
	{
		return foreground * alpha + (1 - alpha) * background;
	}
}