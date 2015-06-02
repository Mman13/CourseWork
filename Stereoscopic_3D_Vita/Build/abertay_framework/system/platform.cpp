#include <system/platform.h>

namespace abfw
{
	Platform::Platform() :
		render_target_(NULL)
	{
	}

	Platform::~Platform()
	{
	}

	std::string Platform::FormatFilename(const std::string& filename) const
	{
		return FormatFilename(filename.c_str());
	}

	RenderTarget* Platform::CreateRenderTarget() const
	{
		return CreateRenderTarget(width(), height());
	}
}