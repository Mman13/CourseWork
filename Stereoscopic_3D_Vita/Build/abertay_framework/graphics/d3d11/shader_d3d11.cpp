#include <abertay_framework.h>
#include <graphics/d3d11/shader_d3d11.h>
#include <graphics/d3d11/texture_d3d11.h>
#include <d3dcompiler.h>

namespace abfw
{

ShaderD3D11::ShaderD3D11(ID3D11Device* device, const char* vertex_shader_string, const int vertex_shader_string_len, const char* pixel_shader_string, const int pixel_shader_string_len, const char* vertex_shader_source_name, const char* pixel_shader_source_name, D3D11_INPUT_ELEMENT_DESC* elements, UInt32 num_elements) :
	vertex_shader_string_(vertex_shader_string),
	pixel_shader_string_(pixel_shader_string),
	vertex_shader_(NULL),
	pixel_shader_(NULL),
	vertex_input_layout_(NULL)
{
	bool success = true;
	// Compile and create the vertex shader
	if(vertex_shader_string_)
	{
		DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	    shader_flags |= D3DCOMPILE_DEBUG;
#endif
		ID3D10Blob* vs_blob = NULL;
		ID3D10Blob* error_blob = NULL;
		HRESULT hresult = S_OK;
		hresult = D3DCompile( vertex_shader_string_, vertex_shader_string_len, vertex_shader_source_name, NULL, NULL, "VS", 
						 "vs_4_0", shader_flags, 0, &vs_blob, &error_blob );
		if( FAILED( hresult ) )
		{
			if( error_blob != NULL )
			{
				OutputDebugStringA( (CHAR*)error_blob->GetBufferPointer() );
				error_blob->Release();
			}
		}
		else
		{
			hresult = device->CreateVertexShader( vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), 
				NULL, &vertex_shader_ );
		}


		// create the vertex input layout
		if(SUCCEEDED(hresult))
		{
			hresult = device->CreateInputLayout( elements, num_elements, vs_blob->GetBufferPointer(),
											   vs_blob->GetBufferSize(), &vertex_input_layout_ );
		}


		if(FAILED(hresult))
		{
			success = false;
			ReleaseNull(vertex_shader_);
		}
		ReleaseNull(error_blob);
		ReleaseNull(vs_blob);
	}


	// Compile and create the pixel shader
	if(success && pixel_shader_string_)
	{
		DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	    shader_flags |= D3DCOMPILE_DEBUG;
#endif
		ID3D10Blob* ps_blob = NULL;
		ID3D10Blob* error_blob = NULL;
		HRESULT hresult = S_OK;
		hresult = D3DCompile( pixel_shader_string_, pixel_shader_string_len, pixel_shader_source_name, NULL, NULL, "PS", 
						 "ps_4_0", shader_flags, 0, &ps_blob, &error_blob );
		if( FAILED( hresult ) )
		{
			if( error_blob != NULL )
			{
				OutputDebugStringA( (CHAR*)error_blob->GetBufferPointer() );
				error_blob->Release();
			}
		}
		else
		{
			hresult = device->CreatePixelShader( ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), 
				NULL, &pixel_shader_ );
		}

		if(FAILED(hresult))
		{
			success = false;
			ReleaseNull(vertex_shader_);
			ReleaseNull(vertex_input_layout_);
			ReleaseNull(pixel_shader_);
		}
		ReleaseNull(error_blob);
		ReleaseNull(ps_blob);
	}
}

ShaderD3D11::~ShaderD3D11()
{
	ReleaseNull(vertex_input_layout_);
	ReleaseNull(pixel_shader_);
	ReleaseNull(vertex_shader_);
}

void ShaderD3D11::Set(ID3D11DeviceContext* device_context)
{
    // Set the vertex and pixel shaders that will be used for rendering.
	device_context->VSSetShader(this->vertex_shader_, NULL, 0);
	device_context->PSSetShader(this->pixel_shader_, NULL, 0);
	device_context->IASetInputLayout( vertex_input_layout_ );
}

void ShaderD3D11::BindTexture(ID3D11DeviceContext* device_context, const TextureD3D11* texture)
{
	if(texture)
		texture->Bind(device_context);
}

void ShaderD3D11::UnbindTexture(ID3D11DeviceContext* device_context, const TextureD3D11* texture)
{
	if(texture)
		texture->Unbind(device_context);
}

HRESULT ShaderD3D11::CreateConstantBuffer(ID3D11Device* device, UInt32 buffer_size, ID3D11Buffer** constant_buffer)
{
    // Setup the description of the dynamic constant buffer that is in the vertex shader.
 	D3D11_BUFFER_DESC constant_buffer_desc;
	constant_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	constant_buffer_desc.ByteWidth = buffer_size;
    constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constant_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constant_buffer_desc.MiscFlags = 0;
	constant_buffer_desc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT hresult = S_OK;
	hresult = device->CreateBuffer(&constant_buffer_desc, NULL, constant_buffer);

	return hresult;
}

}
