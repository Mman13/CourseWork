#ifndef _ABFW_DEFAULT_SHADER_D3D_H
#define _ABFW_DEFAULT_SHADER_D3D_H

#include <graphics/d3d11/shader_d3d11.h>
#include <maths/matrix44.h>
#include <maths/vector4.h>

#define MAX_NUM_POINT_LIGHTS 4

namespace abfw
{
	class TextureD3D11;

	class Default3DShaderD3D11 : public ShaderD3D11
	{
	public:


		Default3DShaderD3D11(ID3D11Device* device, const class Default3DShaderData& shader_data, const UInt32 vs_constant_buffer_size = sizeof(VertexShaderData), const UInt32 ps_constant_buffer_size = sizeof(PixelShaderData));
		~Default3DShaderD3D11();
		void Release();


		void Set(ID3D11DeviceContext* device_context);
		void SetConstantBuffers(ID3D11DeviceContext* device_context, const void* data);

	protected:
		Default3DShaderD3D11(ID3D11Device* device, const char* str_vertex_shader, const int vertex_shader_string_len, const char* str_pixel_shader, const int pixel_shader_string_len, const char* vertex_shader_source_name, const char* pixel_shader_source_name, const class Default3DShaderData& shader_data, const UInt32 vs_constant_buffer_size = sizeof(VertexShaderData), const UInt32 ps_constant_buffer_size = sizeof(PixelShaderData));
		void Init(ID3D11Device* device, UInt32 vs_constant_buffer_size, UInt32 ps_constant_buffer_size);

		ID3D11Buffer* vs_constant_buffer_;
		ID3D11Buffer* ps_constant_buffer_;
		ID3D11SamplerState* sampler_state_;

		const Default3DShaderData& shader_data_;

	private:
		struct VertexShaderData
		{
			Matrix44 wvp;
			Matrix44 world;
			Matrix44 invworld;
			Vector4 light_position[MAX_NUM_POINT_LIGHTS];
		};

		struct PixelShaderData
		{
			Vector4 ambient_light_colour;
			Vector4 light_colour[MAX_NUM_POINT_LIGHTS];
		};

		void SetVertexShaderData(VertexShaderData* vs_data, const void* data);
		void SetPixelShaderData(PixelShaderData* ps_data, const void* data);

	};
}

#endif // _ABFW_DEFAULT_SHADER_D3D_H