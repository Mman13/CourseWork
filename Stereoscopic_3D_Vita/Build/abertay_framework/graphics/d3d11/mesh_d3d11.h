#ifndef _ABFW_MESH_D3D_H
#define _ABFW_MESH_D3D_H

#include <graphics/mesh.h>

#include <d3d11.h>

namespace abfw
{
	class Platform;

	class MeshD3D11 : public Mesh
	{
	public:
		MeshD3D11();
		~MeshD3D11();

		bool InitVertexBuffer(const Platform& platform, const void* vertices, const UInt32 num_vertices, const UInt32 vertex_byte_size, const bool read_only);
		void SetVertexBuffer(ID3D11DeviceContext* device_context) const;
		class Primitive* AllocatePrimitive();
		bool UpdateVertices(Platform& platform, const void* vertices);

	private:
		ID3D11Buffer* vertex_buffer_;
	};
}

#endif // _ABFW_MESH_D3D_H