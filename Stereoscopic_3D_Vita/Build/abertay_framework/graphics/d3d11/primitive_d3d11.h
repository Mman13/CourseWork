#ifndef _ABFW_PRIMITIVE_D3D_H
#define _ABFW_PRIMITIVE_D3D_H

#include <graphics/primitive.h>
#include <d3d11.h>

namespace abfw
{
	class Platform;

	class PrimitiveD3D11 : public Primitive
	{
	public:
		PrimitiveD3D11();
		~PrimitiveD3D11();

		bool InitIndexBuffer(const Platform& platform, const void* indices, const UInt32 num_indices, const UInt32 index_byte_size);
		void SetIndexBuffer(ID3D11DeviceContext* device_context) const;

		void SetType(PrimitiveType type);
		inline D3D11_PRIMITIVE_TOPOLOGY topology() const { return topology_; }


	private:
		ID3D11Buffer*      index_buffer_;
		D3D11_PRIMITIVE_TOPOLOGY topology_;
	};
}

#endif // _ABFW_PRIMITIVE_D3D_H