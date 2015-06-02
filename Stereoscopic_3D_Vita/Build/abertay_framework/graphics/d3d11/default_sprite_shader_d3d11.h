#ifndef _ABFW_DEFAULT_SPRITE_SHADER_D3D_H
#define _ABFW_DEFAULT_SPRITE_SHADER_D3D_H

#include <graphics/d3d11/shader_d3d11.h>
#include <maths/matrix44.h>
#include <maths/vector3.h>


namespace abfw
{


	class DefaultSpriteShaderD3D11 : public ShaderD3D11
	{
	public:
		struct VertexShaderData
		{
			Matrix44 projection_matrix;
			Matrix44 sprite_matrix;
		};

		DefaultSpriteShaderD3D11(ID3D11Device* device);
		DefaultSpriteShaderD3D11(ID3D11Device* device, const char* vertex_shader_string, const int vertex_shader_string_len, const char* pixel_shader_string, const int pixel_shader_string_len, const char* vertex_shader_source_name, const char* pixel_shader_source_name);
		~DefaultSpriteShaderD3D11();
		void Release();

		void Set(ID3D11DeviceContext* device_context);
		void SetConstantBuffers(ID3D11DeviceContext* device_context, const void* data);


//		inline const Vector3& light_position() const { return light_position_; }
//		inline void set_light_position(const Vector3& position) { light_position_ = position; }

	private:
		void Init(ID3D11Device* device);

		ID3D11Buffer* vs_constant_buffer_;
		ID3D11SamplerState* sampler_state_;
//		Vector3 light_position_;
	};
}

#endif // _ABFW_DEFAULT_SPRITE_SHADER_D3D_H