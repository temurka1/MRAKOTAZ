#ifndef DIMENSION_H
#define DIMENSION_H

namespace MRAKOTAZ::core
{
	template <typename T> class ResultOr;

	class Dimension
	{
	public:
		uint32_t width;
		uint32_t height;

		static ResultOr<Dimension> FromString(const std::string& str);
	};
}

#endif
