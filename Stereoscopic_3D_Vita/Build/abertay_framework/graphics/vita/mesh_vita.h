#ifndef _ABFW_MESH_VITA_H
#define _ABFW_MESH_VITA_H

#include <graphics/mesh.h>
#include <gxm.h>

namespace abfw
{
	class MeshVita : public Mesh
	{
	public:
		MeshVita();
		~MeshVita();

		bool InitVertexBuffer(const Platform& platform, const void* vertices, const UInt32 num_vertices, const UInt32 vertex_byte_size, const bool read_only);
		void SetVertexBuffer(SceGxmContext* context) const;
		class Primitive* AllocatePrimitive();
		bool UpdateVertices(Platform& platform, const void* vertices);

	private:
//		ID3D11Buffer* vertex_buffer_;

		SceUID vertices_uid_;
		void* vertices_;

	};
}

#endif // _ABFW_MESH_VITA_H