#include <graphics/d3d11/mesh_d3d11.h>
#include <graphics/d3d11/primitive_d3d11.h>
#include <system/d3d11/platform_d3d11.h>

namespace abfw
{

	MeshD3D11::MeshD3D11() :
		vertex_buffer_(NULL)
	{
	}

	MeshD3D11::~MeshD3D11()
	{
		ReleaseNull(vertex_buffer_);
	}

	bool MeshD3D11::InitVertexBuffer(const Platform& platform, const void* vertices, const UInt32 num_vertices, const UInt32 vertex_byte_size, const bool read_only)
	{

		bool success = Mesh::InitVertexBuffer(platform, vertices, num_vertices, vertex_byte_size, read_only);
		if(success && vertices && num_vertices && vertex_byte_size)
		{
			const PlatformD3D11& platform_d3d = static_cast<const PlatformD3D11&>(platform);

			D3D11_BUFFER_DESC bd;
			// if copy vertex data is set then we're assuming this buffer will be getting updated, so let's make it dynamic
			bd.Usage = read_only ?  D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = vertex_byte_size * num_vertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = read_only ? 0 : D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA init_data;
			init_data.pSysMem = vertices;

			HRESULT hresult = platform_d3d.device()->CreateBuffer( &bd, &init_data, &vertex_buffer_ );
			if(FAILED(hresult))
			{
				success = false;
			}
		}

		return success;
	}

	void MeshD3D11::SetVertexBuffer(ID3D11DeviceContext* device_context) const
	{
		UINT stride = vertex_byte_size_;
		UINT offset = 0;
		device_context->IASetVertexBuffers( 0, 1, &vertex_buffer_, &stride, &offset );
	}

	class Primitive* MeshD3D11::AllocatePrimitive()
	{
		return new PrimitiveD3D11();
	}

	
	bool MeshD3D11::UpdateVertices(class Platform& platform, const void* vertices)
	{
		bool success = true;

		PlatformD3D11& platform_d3d = static_cast<PlatformD3D11&>(platform);


		D3D11_MAPPED_SUBRESOURCE resource;
		HRESULT result = platform_d3d.device_context()->Map(vertex_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		if(result == S_OK)
		{
			resource.pData;
			memcpy(resource.pData, vertices, num_vertices()*vertex_byte_size());
			platform_d3d.device_context()->Unmap(vertex_buffer_, 0);
		}

		return success;
	}

}
