#include "right_eye_3d_shader.h"

// RIGHT EYE CYAN

extern unsigned char default_3d_shader_vs_hlsl[];
extern unsigned int default_3d_shader_vs_hlsl_len;

const char str_right_eye_pixel_shader[]  =
"#define NUM_LIGHTS 4\n"
"\n"
"cbuffer LightBuffer\n"
"{\n"
"   float4 ambient_light_colour;\n"
"   float4 light_colour[NUM_LIGHTS];\n"
"};\n"
"\n"
"struct PixelInput\n"
"{\n"
"    float4 position : SV_POSITION;\n"
"    float3 normal: NORMAL;\n"
"    float2 uv : TEXCOORD0;\n"
"    float3 light_vector1 : TEXCOORD1;\n"
"    float3 light_vector2 : TEXCOORD2;\n"
"    float3 light_vector3 : TEXCOORD3;\n"
"    float3 light_vector4 : TEXCOORD4;\n"
"};\n"
"\n"
"Texture2D diffuse_texture;\n"
"\n"
"SamplerState Sampler0\n"
"{\n"
"    Filter = MIN_MAG_MIP_LINEAR;\n"
"    AddressU = Wrap;\n"
"    AddressV = Wrap;\n"
"};\n"
"float4 PS( PixelInput input ) : SV_Target\n"
"{\n"
"    float diffuse_light1 = saturate(dot(input.normal, input.light_vector1));\n"
"    float diffuse_light2 = saturate(dot(input.normal, input.light_vector2));\n"
"    float diffuse_light3 = saturate(dot(input.normal, input.light_vector3));\n"
"    float diffuse_light4 = saturate(dot(input.normal, input.light_vector4));\n"
"    float4 diffuse_texture_colour = diffuse_texture.Sample( Sampler0, input.uv );\n"
"    float4 diffuse_colour1 = diffuse_light1*light_colour[0];\n"
"    float4 diffuse_colour2 = diffuse_light2*light_colour[1];\n"
"    float4 diffuse_colour3 = diffuse_light3*light_colour[2];\n"
"    float4 diffuse_colour4 = diffuse_light4*light_colour[3];\n"
"    float4 final_colour = saturate(ambient_light_colour+diffuse_colour1+diffuse_colour2+diffuse_colour3+diffuse_colour4)*diffuse_texture_colour;\n"
"    float grayscale = dot(final_colour.rgb, float3(0.3, 0.59, 0.11));\n"
"    final_colour.r = 0; final_colour.g = grayscale; final_colour.b = grayscale;\n"
"    return final_colour;\n"
"}\n";


RightEye3DShader::RightEye3DShader(ID3D11Device* device, const class abfw::Default3DShaderData& shader_data) :
abfw::Default3DShaderD3D11(device, (const char *)default_3d_shader_vs_hlsl, default_3d_shader_vs_hlsl_len, str_right_eye_pixel_shader, sizeof(str_right_eye_pixel_shader), "", "", shader_data)
{
}