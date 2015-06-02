#ifndef _ABFW_SHADER_D3D11_H
#define _ABFW_SHADER_D3D11_H

#include <abertay_framework.h>
#include <graphics/shader.h>

#include <d3d11.h>

namespace abfw
{
	class TextureD3D11;

	class ShaderD3D11 : public Shader
	{
	public:
		ShaderD3D11(ID3D11Device* device, const char* vertex_shader_string, const int vertex_shader_string_len, const char* pixel_shader_string, const int pixel_shader_string_len, const char* vertex_shader_source_name, const char* pixel_shader_source_name, D3D11_INPUT_ELEMENT_DESC* elements, UInt32 num_elements);
		~ShaderD3D11();
		virtual void Set(ID3D11DeviceContext* device_context);
		virtual void SetConstantBuffers(ID3D11DeviceContext* device_context, const void* data) = 0;

		virtual void BindTexture(ID3D11DeviceContext* device_context, const TextureD3D11* texture);
		virtual void UnbindTexture(ID3D11DeviceContext* device_context, const TextureD3D11* texture);

	protected:
		HRESULT CreateConstantBuffer(ID3D11Device* device, UInt32 buffer_size, ID3D11Buffer** constant_buffer);

		ID3D11VertexShader* vertex_shader_;
		ID3D11PixelShader* pixel_shader_;
		ID3D11InputLayout* vertex_input_layout_;
	private:
		const char* vertex_shader_string_;
		const char* pixel_shader_string_;
	};
}
#endif // _ABFW_SHADER_D3D11_H