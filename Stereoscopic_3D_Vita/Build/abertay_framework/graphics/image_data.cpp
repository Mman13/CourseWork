#include <graphics/image_data.h>
#include <cstdlib>

namespace abfw
{
	ImageData::ImageData() :
		image_(NULL),
		clut_(NULL)
	{
	}

	ImageData::~ImageData()
	{
		delete image_;
		delete clut_;
	}
}