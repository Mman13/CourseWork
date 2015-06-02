#include "s3d_shader_d3d11.h"
#include <graphics/d3d11/texture_d3d11.h>

extern unsigned char textured_sprite_vs_hlsl[];
extern unsigned int textured_sprite_vs_hlsl_len;
#include "shaders/s3d_ps.h"

S3DShaderD3D11::S3DShaderD3D11(ID3D11Device* device) :
abfw::DefaultSpriteShaderD3D11(device, (const char*)textured_sprite_vs_hlsl, textured_sprite_vs_hlsl_len, (const char*)s3d_ps_hlsl, s3d_ps_hlsl_len, "textured_sprite_vs.hlsl", "s3d_ps.hlsl"),
		left_eye_texture_(NULL),
		right_eye_texture_(NULL)
{
}

void S3DShaderD3D11::BindTexture(ID3D11DeviceContext* device_context, const abfw::TextureD3D11* texture)
{
	static_cast<const abfw::TextureD3D11*>(left_eye_texture_)->Bind(device_context, 0);
	static_cast<const abfw::TextureD3D11*>(right_eye_texture_)->Bind(device_context, 1);
}

void S3DShaderD3D11::UnbindTexture(ID3D11DeviceContext* device_context, const abfw::TextureD3D11* texture)
{
	static_cast<const abfw::TextureD3D11*>(left_eye_texture_)->Unbind(device_context, 0);
	static_cast<const abfw::TextureD3D11*>(right_eye_texture_)->Unbind(device_context, 1);
}
