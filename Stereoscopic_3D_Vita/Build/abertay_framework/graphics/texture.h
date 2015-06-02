#ifndef _ABFW_TEXTURE_H
#define _ABFW_TEXTURE_H

#include <abertay_framework.h>

namespace abfw
{
	class ImageData;
	class Platform;

class Texture
{
public:
	Texture();
	Texture(const Platform& platform, const ImageData& image_data);
	virtual ~Texture();

	static Texture* CreateCheckerTexture(const Int32 size, const Int32 num_checkers, const Platform& platform);
};

}
#endif // _ABFW_TEXTURE_H