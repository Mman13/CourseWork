#ifndef _ABFW_SPRITE_RENDERER_D3D11_H
#define _ABFW_SPRITE_RENDERER_D3D11_H

#include <graphics/sprite_renderer.h>
#include <maths/vector3.h>
#include <maths/matrix44.h>
#include <d3d11.h>

namespace abfw
{
	class Sprite;
	class DefaultSpriteShaderD3D11;
	class TextureD3D11;

	struct SpriteVertex
	{
		Vector3 position;
	};

	class SpriteRendererD3D11 : public SpriteRenderer
	{
	public:
		SpriteRendererD3D11(const class Platform& platform);
		~SpriteRendererD3D11();
		void Begin(bool clear);
		void End();
		void DrawSprite(const Sprite& sprite);
	private:
		void CleanUp();

		ID3D11RasterizerState* default_render_state_;
		ID3D11BlendState* default_blend_state_;
		ID3D11DepthStencilState* default_depth_stencil_state_;
		ID3D11Buffer* vertex_buffer_;
		ID3D11Buffer*      index_buffer_;
		TextureD3D11* default_texture_;
	};

}
#endif // _ABFW_SPRITE_RENDERER_D3D11_H