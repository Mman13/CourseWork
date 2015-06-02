#ifndef _S3D_SHADER_D3D11_H
#define _S3D_SHADER_D3D11_H

#include <graphics/d3d11/default_sprite_shader_d3d11.h>

namespace abfw
{
	class Texture;
	class TextureD3D11;
}

class S3DShaderD3D11 : public abfw::DefaultSpriteShaderD3D11
{
public:
	S3DShaderD3D11(ID3D11Device* device);

	inline void set_left_eye_texture(const abfw::Texture* texture) { left_eye_texture_ = texture; }
	inline void set_right_eye_texture(const abfw::Texture* texture) { right_eye_texture_ = texture; }

	void BindTexture(ID3D11DeviceContext* device_context, const abfw::TextureD3D11* texture);
	void UnbindTexture(ID3D11DeviceContext* device_context, const abfw::TextureD3D11* texture);

private:

	const abfw::Texture* left_eye_texture_;
	const abfw::Texture* right_eye_texture_;
};

#endif // _S3D_SHADER_D3D11_H