#include <graphics/d3d11/primitive_d3d11.h>
#include <system/d3d11/platform_d3d11.h>

namespace abfw
{
PrimitiveD3D11::PrimitiveD3D11() :
	index_buffer_(NULL),
	topology_(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED)
{
}

PrimitiveD3D11::~PrimitiveD3D11()
{
	ReleaseNull(index_buffer_);
}

bool PrimitiveD3D11::InitIndexBuffer(const Platform& platform, const void* indices, const UInt32 num_indices, const UInt32 index_byte_size)
{
	num_indices_ = num_indices;
	const PlatformD3D11& platform_d3d = static_cast<const PlatformD3D11&>(platform);
	bool success = true;

    D3D11_BUFFER_DESC bd;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = index_byte_size * num_indices_;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA init_data;
    init_data.pSysMem = indices;
	HRESULT hresult = platform_d3d.device()->CreateBuffer( &bd, &init_data, &index_buffer_ );
	if(FAILED(hresult))
	{
		success = false;
	}

	return success;
}

void PrimitiveD3D11::SetIndexBuffer(ID3D11DeviceContext* device_context) const
{
	if(index_buffer_)
	{
	    // Set index buffer
	    device_context->IASetIndexBuffer( index_buffer_, DXGI_FORMAT_R32_UINT, 0 );
	}
}

void PrimitiveD3D11::SetType(PrimitiveType type)
{
	type_ = type;

	switch(type_)
	{
	case TRIANGLE_LIST:
		topology_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;

	case LINE_LIST:
		topology_ = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;

	case TRIANGLE_STRIP:
		topology_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	}
}
}