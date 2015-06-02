#ifndef _ABFW_PNG_LOADER_H
#define _ABFW_PNG_LOADER_H

#include <abertay_framework.h>

struct png_struct_def;
struct png_info_def;

namespace abfw
{
	// forward declarations
	class Platform;
	class ImageData;

	class PNGLoader
	{
	public:
		PNGLoader();
		~PNGLoader();

		void Load(const char* filename, const Platform& platform, ImageData& image_data);
	private:
		void ParseRGBA(UInt8* out_image_buffer, png_struct_def* png_ptr, const png_info_def* info_ptr, UInt32 width, UInt32 height);
		UInt32 NextPowerOfTwo(const UInt32 value);
	};

}

#endif // _ABFW_PNG_LOADER_H