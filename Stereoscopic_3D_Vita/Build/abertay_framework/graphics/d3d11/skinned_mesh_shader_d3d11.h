#ifndef _ABFW_SKINNED_MESH_SHADER_D3D11_H
#define _ABFW_SKINNED_MESH_SHADER_D3D11_H

#include <graphics/d3d11/shader_d3d11.h>
#include <graphics/skinned_mesh_shader_data.h>
#include <maths/matrix44.h>

#define MAX_NUM_BONE_MATRICES 128
#define MAX_NUM_POINT_LIGHTS 4

namespace abfw
{
	class SkinnedMeshShaderData;

	class SkinnedMeshShaderD3D11 : public ShaderD3D11
	{
	public:


		SkinnedMeshShaderD3D11(ID3D11Device* device, const SkinnedMeshShaderData& shader_data);
		~SkinnedMeshShaderD3D11();

		void Set(ID3D11DeviceContext* device_context);
		void SetConstantBuffers(ID3D11DeviceContext* device_context, const void* data);

	protected:
		struct BoneMatricesData
		{
			abfw::Matrix44 bone_matrices[MAX_NUM_BONE_MATRICES];
		};

		struct ModelMatricesData
		{
			Matrix44 wvp;
			Matrix44 world;
		};

		struct LightPositionData
		{
			Vector4 light_position[MAX_NUM_POINT_LIGHTS];
		};

		struct LightColourData
		{
			Vector4 ambient_light_colour;
			Vector4 light_colour[MAX_NUM_POINT_LIGHTS];
		};

		inline const SkinnedMeshShaderData& skinned_mesh_shader_data() { return static_cast<const SkinnedMeshShaderData&>(shader_data_); }

	private:
//		void SetVertexShaderData(void* vs_data, const void* data);

		ID3D11Buffer* bone_matrices_cbuffer_;
		ID3D11Buffer* model_matrices_cbuffer_;
		ID3D11Buffer* light_position_cbuffer_;
		ID3D11Buffer* light_colour_cbuffer_;
		ID3D11SamplerState* sampler_state_;


		const SkinnedMeshShaderData& shader_data_;
	};
}

#endif // _ABFW_SKINNED_MESH_SHADER_D3D11_H