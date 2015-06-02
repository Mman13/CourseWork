#ifndef _ABFW_RENDERER_3D_VITA_H
#define _ABFW_RENDERER_3D_VITA_H

#include <graphics/renderer_3d.h>

namespace abfw
{
	class Platform;
	class MeshInstance;
	class ShaderVita;
	class TextureVita;

	class Renderer3DVita : public Renderer3D
	{
	public:
		Renderer3DVita(const Platform& platform);
		~Renderer3DVita();
		void CleanUp();

		void Begin(bool clear);
		void End();
		void DrawMesh(const class MeshInstance& mesh_instance, bool overwrite_world_matrix);
		void ClearZBuffer();
	private:
		TextureVita* default_texture_;
	};
}

#endif // _ABFW_RENDERER_3D_VITA_H