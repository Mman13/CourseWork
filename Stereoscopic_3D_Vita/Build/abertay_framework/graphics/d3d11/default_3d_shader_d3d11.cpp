#include <graphics/d3d11/default_3d_shader_d3d11.h>
#include <graphics/d3d11/renderer_3d_d3d11.h>
#include <graphics/d3d11/texture_d3d11.h>
#include <maths/vector3.h>
#include <graphics/colour.h>

#include <graphics/d3d11/shaders/default_3d_shader_vs.h>
#include <graphics/d3d11/shaders/default_3d_shader_ps.h>

// Create the input layout
static D3D11_INPUT_ELEMENT_DESC elements[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
static UINT numElements = _countof( elements );

namespace abfw
{

Default3DShaderD3D11::Default3DShaderD3D11(ID3D11Device* device, const class Default3DShaderData& shader_data, const UInt32 vs_constant_buffer_size, const UInt32 ps_constant_buffer_size) :
ShaderD3D11(device, (const char*)default_3d_shader_vs_hlsl, default_3d_shader_vs_hlsl_len, (const char*)default_3d_shader_ps_hlsl, default_3d_shader_ps_hlsl_len, "default_3d_shader_vs.hlsl", "default_3d_shader_ps.hlsl", elements, numElements),
	shader_data_(shader_data),
	vs_constant_buffer_(NULL),
	ps_constant_buffer_(NULL),
	sampler_state_(NULL)

{
	Init(device, vs_constant_buffer_size, ps_constant_buffer_size);
}

Default3DShaderD3D11::Default3DShaderD3D11(ID3D11Device* device, const char* str_vertex_shader, const int vertex_shader_string_len, const char* str_pixel_shader, const int pixel_shader_string_len, const char* vertex_shader_source_name, const char* pixel_shader_source_name, const class Default3DShaderData& shader_data, const UInt32 vs_constant_buffer_size, const UInt32 ps_constant_buffer_size) :
ShaderD3D11(device, (const char*)str_vertex_shader, vertex_shader_string_len, (const char*)str_pixel_shader, pixel_shader_string_len, vertex_shader_source_name, pixel_shader_source_name, elements, numElements),
	shader_data_(shader_data),
	vs_constant_buffer_(NULL),
	ps_constant_buffer_(NULL),
	sampler_state_(NULL)
{
	Init(device, vs_constant_buffer_size, ps_constant_buffer_size);
}


void Default3DShaderD3D11::Init(ID3D11Device* device, UInt32 vs_constant_buffer_size, UInt32 ps_constant_buffer_size)
{
//	 set_light_position(Vector3(0.0f, 1000.0f, 1000.0f));
	HRESULT hresult = CreateConstantBuffer(device, vs_constant_buffer_size, &vs_constant_buffer_);

	if(SUCCEEDED(hresult))
	{
		hresult = CreateConstantBuffer(device, ps_constant_buffer_size, &ps_constant_buffer_);
	}

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




Default3DShaderD3D11::~Default3DShaderD3D11()
{
	Release();
}

void Default3DShaderD3D11::Release()
{
	ReleaseNull(vs_constant_buffer_);
	ReleaseNull(ps_constant_buffer_);
	ReleaseNull(sampler_state_);
}

void Default3DShaderD3D11::Set(ID3D11DeviceContext* device_context)
{
	ShaderD3D11::Set(device_context);
	// Set the sampler state in the pixel shader.
	device_context->PSSetSamplers(0, 1, &sampler_state_);
}

void Default3DShaderD3D11::SetConstantBuffers(ID3D11DeviceContext* device_context, const void* data)
{
	HRESULT hresult;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

	const Renderer3DD3D11* renderer = static_cast<const Renderer3DD3D11*>(data);

	// Lock the constant buffer so it can be written to.
	hresult = device_context->Map(vs_constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	VertexShaderData* vs_data = NULL;
	PixelShaderData* ps_data = NULL;

	// Get a pointer to the data in the constant buffer.
	if(SUCCEEDED(hresult))
		vs_data = (VertexShaderData*)mappedResource.pData;

	if(SUCCEEDED(hresult))
	{
		hresult = device_context->Map(ps_constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		ps_data = (PixelShaderData*)mappedResource.pData;
	}

	if(SUCCEEDED(hresult))
	{
		SetVertexShaderData(vs_data, data);
		SetPixelShaderData(ps_data, data);

		// Unlock the constant buffer.
		device_context->Unmap(vs_constant_buffer_, 0);
		device_context->Unmap(ps_constant_buffer_, 0);

		// Set the position of the constant buffer in the vertex shader.
		UInt32 buffer_number = 0;

		// Now set the constant buffer in the vertex shader with the updated values.
		device_context->VSSetConstantBuffers(buffer_number, 1, &vs_constant_buffer_);
		device_context->PSSetConstantBuffers(buffer_number, 1, &ps_constant_buffer_);
	}
}

void Default3DShaderD3D11::SetVertexShaderData(VertexShaderData* vs_data, const void* data)
{
	const Renderer3DD3D11* renderer = static_cast<const Renderer3DD3D11*>(data);

	// Copy the matrices into the constant buffer.
	vs_data->invworld.Transpose(renderer->inv_world_transpose_matrix());
	vs_data->world.Transpose(renderer->world_matrix());
	vs_data->wvp.Transpose(renderer->world_matrix()*renderer->view_matrix()*renderer->projection_matrix());

	// light position
	for(UInt32 light_num=0;light_num < MAX_NUM_POINT_LIGHTS; ++light_num)
	{
		Vector3 light_position;
		if(light_num < shader_data_.GetNumPointLights())
		{
			const PointLight& point_light = shader_data_.GetPointLight(light_num);
			light_position = point_light.position();
		}
		else
		{
			// no light data
			// set this light to a light with no colour
			light_position = Vector3(0.0f, 0.0f, 0.0f);
		}
		vs_data->light_position[light_num] = Vector4(light_position.x, light_position.y, light_position.z, 1.f);
	}

}

void Default3DShaderD3D11::SetPixelShaderData(PixelShaderData* ps_data, const void* data)
{
	const Renderer3DD3D11* renderer = static_cast<const Renderer3DD3D11*>(data);

	ps_data->ambient_light_colour = shader_data_.ambient_light_colour().GetRGBAasVector4();
	// light position
	for(UInt32 light_num=0;light_num < MAX_NUM_POINT_LIGHTS; ++light_num)
	{
		Colour light_colour;
		if(light_num < shader_data_.GetNumPointLights())
		{
			const PointLight& point_light = shader_data_.GetPointLight(light_num);
			light_colour = point_light.colour();
		}
		else
		{
			// no light data
			// set this light to a light with no colour
			light_colour = Colour(0.0f, 0.0f, 0.0f);
		}
		ps_data->light_colour[light_num] = light_colour.GetRGBAasVector4();
	}
}


}