#include <graphics/d3d11/skinned_mesh_shader_d3d11.h>
#include <graphics/skinned_mesh_shader_data.h>
#include <graphics/d3d11/renderer_3d_d3d11.h>

#include <graphics/d3d11/shaders/skinning_shader_ps.h>
#include <graphics/d3d11/shaders/skinning_shader_vs.h>

// Create the input layout
static D3D11_INPUT_ELEMENT_DESC elements[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BLENDINDICES", 0, DXGI_FORMAT_R32_UINT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
static UINT numElements = _countof( elements );

namespace abfw
{
	SkinnedMeshShaderD3D11::SkinnedMeshShaderD3D11(ID3D11Device* device, const SkinnedMeshShaderData& shader_data) :
ShaderD3D11(device, (const char*)skinning_shader_vs_hlsl, skinning_shader_vs_hlsl_len, (const char*)skinning_shader_ps_hlsl, skinning_shader_ps_hlsl_len, "skinning_shader_vs.hlsl", "skinning_shader_shader_ps.hlsl", elements, numElements),
			model_matrices_cbuffer_(NULL),
			light_position_cbuffer_(NULL),
			light_colour_cbuffer_(NULL),
			bone_matrices_cbuffer_(NULL),
			shader_data_(shader_data),
			sampler_state_(NULL)
	{
		HRESULT hresult = S_OK;

		hresult = CreateConstantBuffer(device, sizeof(ModelMatricesData), &model_matrices_cbuffer_);
		hresult = CreateConstantBuffer(device, sizeof(LightPositionData), &light_position_cbuffer_);
		hresult = CreateConstantBuffer(device, sizeof(LightColourData), &light_colour_cbuffer_);
		hresult = CreateConstantBuffer(device, sizeof(BoneMatricesData), &bone_matrices_cbuffer_);

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
	}

	SkinnedMeshShaderD3D11::~SkinnedMeshShaderD3D11()
	{
		ReleaseNull(model_matrices_cbuffer_);
		ReleaseNull(light_position_cbuffer_);
		ReleaseNull(light_colour_cbuffer_);
		ReleaseNull(bone_matrices_cbuffer_);
		ReleaseNull(sampler_state_);
	}

	void SkinnedMeshShaderD3D11::SetConstantBuffers(ID3D11DeviceContext* device_context, const void* data)
	{
		HRESULT hresult = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		const Renderer3DD3D11* renderer = static_cast<const Renderer3DD3D11*>(data);

		ModelMatricesData* model_matrices_data = NULL;
		LightPositionData* light_position_data = NULL;
		LightColourData* light_colour_data = NULL;
		BoneMatricesData* bone_matrices_data = NULL;

		if(SUCCEEDED(hresult))
		{
			hresult = device_context->Map(model_matrices_cbuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if(SUCCEEDED(hresult))
				model_matrices_data = (ModelMatricesData*)mappedResource.pData;
		}

		if(SUCCEEDED(hresult))
		{
			hresult = device_context->Map(light_position_cbuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if(SUCCEEDED(hresult))
				light_position_data = (LightPositionData*)mappedResource.pData;
		}

		if(SUCCEEDED(hresult))
		{
			hresult = device_context->Map(light_colour_cbuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if(SUCCEEDED(hresult))
				light_colour_data = (LightColourData*)mappedResource.pData;
		}

		if(SUCCEEDED(hresult))
		{
			hresult = device_context->Map(bone_matrices_cbuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if(SUCCEEDED(hresult))
				bone_matrices_data = (BoneMatricesData*)mappedResource.pData;
		}


		if(SUCCEEDED(hresult))
		{
			// Copy the matrices into the constant buffer.
			model_matrices_data->world.Transpose(renderer->world_matrix());
			model_matrices_data->wvp.Transpose(renderer->world_matrix()*renderer->view_matrix()*renderer->projection_matrix());

			// light position
			for(UInt32 light_num=0;light_num < MAX_NUM_POINT_LIGHTS; ++light_num)
			{
				Vector3 light_position;
				Colour light_colour;
				if(light_num < shader_data_.GetNumPointLights())
				{
					const PointLight& point_light = shader_data_.GetPointLight(light_num);
					light_position = point_light.position();
					light_colour = point_light.colour();
				}
				else
				{
					// no light data
					// set this light to a light with no colour
					light_position = Vector3(0.0f, 0.0f, 0.0f);
					light_colour = Colour(0.0f, 0.0f, 0.0f);
				}

				light_position_data->light_position[light_num] = Vector4(light_position.x, light_position.y, light_position.z, 1.f);
				light_colour_data->light_colour[light_num] = light_colour.GetRGBAasVector4();
			}
			light_colour_data->ambient_light_colour = shader_data_.ambient_light_colour().GetRGBAasVector4();

			// bone matrices
			if(shader_data_.bone_matrices())
			{
				Int32 matrix_num=0;
				for(std::vector<Matrix44>::const_iterator bone_matrix_iter = shader_data_.bone_matrices()->begin(); bone_matrix_iter != shader_data_.bone_matrices()->end(); ++bone_matrix_iter, ++matrix_num)
					bone_matrices_data->bone_matrices[matrix_num].Transpose(*bone_matrix_iter);
			}

			// Unlock the constant buffer.
			device_context->Unmap(model_matrices_cbuffer_, 0);
			device_context->Unmap(light_position_cbuffer_, 0);
			device_context->Unmap(light_colour_cbuffer_, 0);
			device_context->Unmap(bone_matrices_cbuffer_, 0);

			// Set the position of the constant buffer in the vertex shader.
			UInt32 buffer_number = 0;

			// Now set the constant buffer in the vertex shader with the updated values.
			device_context->VSSetConstantBuffers(0, 1, &model_matrices_cbuffer_);
			device_context->VSSetConstantBuffers(1, 1, &light_position_cbuffer_);
			device_context->VSSetConstantBuffers(2, 1, &bone_matrices_cbuffer_);
			device_context->PSSetConstantBuffers(0, 1, &light_colour_cbuffer_);
		}
	}

	void SkinnedMeshShaderD3D11::Set(ID3D11DeviceContext* device_context)
	{
		ShaderD3D11::Set(device_context);
		// Set the sampler state in the pixel shader.
		device_context->PSSetSamplers(0, 1, &sampler_state_);
	}
}