#ifndef _RENDER_TARGET_VITA_H
#define _RENDER_TARGET_VITA_H

#include <graphics/render_target.h>
#include <gxm.h>

namespace abfw
{
	class TextureVita;

	class RenderTargetVita : public RenderTarget
	{
	public:
		RenderTargetVita(const Platform& platform, const Int32 width, const Int32 height);
		~RenderTargetVita();

		void Begin(const Platform& platform);
		void End(const Platform& platform);

		const Texture* texture() const
		{
			return texture_;
		}

	private:
		SceGxmRenderTarget *render_target_;
		SceGxmColorSurface colour_surface_;
		SceGxmDepthStencilSurface depth_surface_;
		SceUID depth_buffer_uid_;
	};
}



#endif // _RENDER_TARGET_VITA_H