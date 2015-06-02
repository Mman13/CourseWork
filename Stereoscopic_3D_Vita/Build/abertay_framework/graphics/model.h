#ifndef _ABFW_MODEL_H
#define _ABFW_MODEL_H

#include <abertay_framework.h>
#include <vector>

namespace abfw
{
	class Mesh;
	class Texture;

	class Model
	{
	public:
		Model();
		~Model();
		void Release();

		inline void set_mesh(Mesh* mesh) { mesh_ = mesh; }
		inline Mesh* mesh() { return mesh_; }
		inline void set_textures(const std::vector<Texture*>& textures) { textures_ = textures; }
	private:
		Mesh* mesh_;
		std::vector<Texture*> textures_;
	};
}

#endif // _ABFW_MODEL_H