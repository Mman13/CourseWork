#include <graphics/vita/mesh_vita.h>
#include <graphics/vita/primitive_vita.h>
#include <system/vita/platform_vita.h>

void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid);
void graphicsFree(SceUID uid);

namespace abfw
{

	MeshVita::MeshVita() :
		vertices_(NULL),
		vertices_uid_(0)
	{
	}

	MeshVita::~MeshVita()
	{
		graphicsFree(vertices_uid_);
	}

	bool MeshVita::InitVertexBuffer(const Platform& platform, const void* vertices, const UInt32 num_vertices, const UInt32 vertex_byte_size, const bool read_only)
	{
		// if copy vertex data is set then we're assuming this buffer will be getting updated, so let's make it dynamic

		bool success = Mesh::InitVertexBuffer(platform, vertices, num_vertices, vertex_byte_size, read_only);
		if(success && vertices && num_vertices && vertex_byte_size)
		{
//			PlatformVita& platform_vita = static_cast<PlatformVita&>(platform);

			// create shaded triangle vertex/index data
			vertices_ = graphicsAlloc(
				SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
				num_vertices*vertex_byte_size,
				4,
				SCE_GXM_MEMORY_ATTRIB_READ,
				&vertices_uid_);

			memcpy(vertices_, vertices, num_vertices*vertex_byte_size);
		}

		return success;
	}

	void MeshVita::SetVertexBuffer(SceGxmContext* context) const
	{
		sceGxmSetVertexStream(context, 0, vertices_);

//		UINT stride = vertex_byte_size_;
//		UINT offset = 0;
//		device_context->IASetVertexBuffers( 0, 1, &vertex_buffer_, &stride, &offset );
	}

	class Primitive* MeshVita::AllocatePrimitive()
	{
		return new PrimitiveVita();
	}

	
	bool MeshVita::UpdateVertices(class Platform& platform, const void* vertices)
	{
		bool success = true;

/*
		PlatformVita& platform_vita = static_cast<PlatformVita&>(platform);

		D3D11_MAPPED_SUBRESOURCE resource;
		HRESULT result = platform_d3d.device_context()->Map(vertex_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		if(result == S_OK)
		{
			resource.pData;
			memcpy(resource.pData, vertices, num_vertices()*vertex_byte_size());
			platform_d3d.device_context()->Unmap(vertex_buffer_, 0);
		}
*/
		return success;
	}

}
