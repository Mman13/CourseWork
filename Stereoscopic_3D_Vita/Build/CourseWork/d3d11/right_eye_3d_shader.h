#ifndef _RIGHT_EYE_SHADER_H
#define _RIGHT_EYE_SHADER_H

#include <graphics/d3d11/default_3d_shader_d3d11.h>

namespace abfw
{
	class Default3DShaderData;
}

class RightEye3DShader : public abfw::Default3DShaderD3D11
{
public:
	RightEye3DShader(ID3D11Device* device, const class abfw::Default3DShaderData& shader_data);
};

#endif // _RIGHT_EYE_SHADER_H