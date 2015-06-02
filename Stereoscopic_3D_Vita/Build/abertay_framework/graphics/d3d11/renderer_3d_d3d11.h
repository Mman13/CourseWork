#ifndef _ABFW_RENDERER_3D_D3D_H
#define _ABFW_RENDERER_3D_D3D_H

#include <graphics/renderer_3d.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11.h>

namespace abfw
{
	class Platform;
	class MeshInstance;
	class ShaderD3D11;
	class TextureD3D11;

	class Renderer3DD3D11 : public Renderer3D
	{
	public:
		Renderer3DD3D11(const Platform& platform);
		~Renderer3DD3D11();
		void CleanUp();

		void Begin(bool clear);
		void End();
		void DrawMesh(const class MeshInstance& mesh_instance, bool overwrite_world_matrix);

		void ClearZBuffer();
	private:
		ID3D11RasterizerState* default_render_state_;
		ID3D11BlendState* default_blend_state_;
		TextureD3D11* default_texture_;

		ID3D11DepthStencilState* default_depth_stencil_state_;
	};
}

#endif // _ABFW_RENDERER_3D_D3D_H