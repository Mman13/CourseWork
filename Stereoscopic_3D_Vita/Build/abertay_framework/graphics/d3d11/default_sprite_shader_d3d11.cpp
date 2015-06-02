#include <abertay_framework.h>
#include <graphics/d3d11/default_sprite_shader_d3d11.h>
#include <graphics/d3d11/texture_d3d11.h>
#include <graphics/d3d11/shaders/textured_sprite_vs.h>
#include <graphics/d3d11/shaders/textured_sprite_ps.h>

// Create the input layout
static D3D11_INPUT_ELEMENT_DESC elements[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
static UINT numElements = _countof( elements );


namespace abfw
{

DefaultSpriteShaderD3D11::DefaultSpriteShaderD3D11(ID3D11Device* device) :
ShaderD3D11(device, (const char*)textured_sprite_vs_hlsl, textured_sprite_vs_hlsl_len, (const char*)textured_sprite_ps_hlsl, textured_sprite_ps_hlsl_len, "textured_sprite_vs.hlsl", "textured_sprite_ps.hlsl", elements, numElements),
	vs_constant_buffer_(NULL),
	sampler_state_(NULL)
{
	Init(device);
}


DefaultSpriteShaderD3D11::DefaultSpriteShaderD3D11(ID3D11Device* device, const char* vertex_shader_string, const int vertex_shader_string_len, const char* pixel_shader_string, const int pixel_shader_string_len, const char* vertex_shader_source_name, const char* pixel_shader_source_name) :
	ShaderD3D11(device, vertex_shader_string, vertex_shader_string_len, pixel_shader_string, pixel_shader_string_len, vertex_shader_source_name, pixel_shader_source_name, elements, numElements),
	vs_constant_buffer_(NULL),
	sampler_state_(NULL)
{
	Init(device);
}


DefaultSpriteShaderD3D11::~DefaultSpriteShaderD3D11()
{
	Release();
}

void DefaultSpriteShaderD3D11::Init(ID3D11Device* device)
{
//	 set_light_position(Vector3(0.0f, 1000.0f, 1000.0f));

    // Setup the description of the dynamic constant buffer that is in the vertex shader.
 	D3D11_BUFFER_DESC constant_buffer_desc;
	constant_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	constant_buffer_desc.ByteWidth = sizeof(VertexShaderData);
    constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constant_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constant_buffer_desc.MiscFlags = 0;
	constant_buffer_desc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT hresult = S_OK;
	hresult = device->CreateBuffer(&constant_buffer_desc, NULL, &this->vs_constant_buffer_);

	if(SUCCEEDED(hresult))
	{
	    D3D11_SAMPLER_DESC sampler_desc;

		// Create a texture sampler state description.
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		hresult = device->CreateSamplerState(&sampler_desc, &sampler_state_);
	}

	if(FAILED(hresult))
	{
		Release();
	}
}


void DefaultSpriteShaderD3D11::Release()
{
	ReleaseNull(vs_constant_buffer_);
	ReleaseNull(sampler_state_);
}

void DefaultSpriteShaderD3D11::Set(ID3D11DeviceContext* device_context)
{
	ShaderD3D11::Set(device_context);
	// Set the sampler state in the pixel shader.
	device_context->PSSetSamplers(0, 1, &sampler_state_);
}

void DefaultSpriteShaderD3D11::SetConstantBuffers(ID3D11DeviceContext* device_context, const void* data)
{
	HRESULT hresult;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

	const VertexShaderData* sprite_shader_data = static_cast<const VertexShaderData*>(data);

	// Lock the constant buffer so it can be written to.
	hresult = device_context->Map(vs_constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if(SUCCEEDED(hresult))
	{
		// Get a pointer to the data in the constant buffer.
		VertexShaderData* data = (VertexShaderData*)mappedResource.pData;

		// Copy the matrices into the constant buffer.
		data->sprite_matrix.Transpose(sprite_shader_data->sprite_matrix);
		data->projection_matrix.Transpose(sprite_shader_data->projection_matrix);

		// Unlock the constant buffer.
		device_context->Unmap(vs_constant_buffer_, 0);

		// Set the position of the constant buffer in the vertex shader.
		UInt32 buffer_number = 0;

		// Now set the constant buffer in the vertex shader with the updated values.
		device_context->VSSetConstantBuffers(buffer_number, 1, &vs_constant_buffer_);
	}
}



}