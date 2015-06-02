#include <graphics/d3d11/renderer_3d_d3d11.h>
#include <system/d3d11/platform_d3d11.h>
#include <graphics/d3d11/default_3d_shader_d3d11.h>
#include <graphics/d3d11/skinned_mesh_shader_d3d11.h>
#include <graphics/d3d11/texture_d3d11.h>
#include <graphics/d3d11/mesh_d3d11.h>
#include <graphics/d3d11/primitive_d3d11.h>
#include <graphics/mesh_instance.h>
#include <graphics/colour.h>
#include <graphics/skinned_mesh_shader_data.h>

namespace abfw
{
	class Platform;

	Renderer3DD3D11::Renderer3DD3D11(const Platform& platform) :
		Renderer3D(platform),
		default_render_state_(NULL),
		default_blend_state_(NULL),
		default_depth_stencil_state_(NULL)
	{
		PointLight default_point_light;
		default_point_light.set_colour(Colour(1.0f, 1.0f, 1.0f, 1.0f));
		default_point_light.set_position(Vector3(0.0f, 1000.0f, 1000.0f));
		default_shader_data_.AddPointLight(default_point_light);

		const PlatformD3D11& platform_d3d = static_cast<const PlatformD3D11&>(platform);
		default_shader_ = new Default3DShaderD3D11(platform_d3d.device(), default_shader_data_);
		shader_ = default_shader_;


		default_skinned_mesh_shader_ = new SkinnedMeshShaderD3D11(platform_d3d.device(), default_skinned_mesh_shader_data_);

		default_texture_ = static_cast<TextureD3D11*>(Texture::CreateCheckerTexture(64, 8, platform));

		HRESULT hresult = S_OK;

		D3D11_RASTERIZER_DESC render_state_desc;
		ZeroMemory(&render_state_desc, sizeof(D3D11_RASTERIZER_DESC));
		render_state_desc.FillMode = D3D11_FILL_SOLID;
		render_state_desc.CullMode = D3D11_CULL_BACK;

		hresult = platform_d3d.device()->CreateRasterizerState(&render_state_desc, &default_render_state_);


		if(SUCCEEDED(hresult))
		{

			D3D11_RENDER_TARGET_BLEND_DESC rtbd;
			ZeroMemory( &rtbd, sizeof(rtbd) );
			rtbd.BlendEnable = true;
			rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			rtbd.BlendOp = D3D11_BLEND_OP_ADD;
			rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
			rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
			rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rtbd.RenderTargetWriteMask = 0x0f;

			D3D11_BLEND_DESC blendDesc;
			ZeroMemory( &blendDesc, sizeof(blendDesc) );
			blendDesc.RenderTarget[0] = rtbd;

			hresult = platform_d3d.device()->CreateBlendState(&blendDesc, &default_blend_state_);
		}

		if(SUCCEEDED(hresult))
		{
			D3D11_DEPTH_STENCIL_DESC dsDesc;

			ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

			// Depth test parameters
			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			// Stencil test parameters
			dsDesc.StencilEnable = false;

			// Create depth stencil state
			platform_d3d.device()->CreateDepthStencilState(&dsDesc, &default_depth_stencil_state_);
		}


		if(FAILED(hresult))
		{
			CleanUp();
		}
	}

	Renderer3DD3D11::~Renderer3DD3D11()
	{
		CleanUp();
	}

	void Renderer3DD3D11::CleanUp()
	{
		DeleteNull(default_texture_);
		ReleaseNull(default_render_state_);
		ReleaseNull(default_blend_state_);
		ReleaseNull(default_depth_stencil_state_);
	}


	void Renderer3DD3D11::Begin(bool clear)
	{
		const PlatformD3D11& platform_d3d = static_cast<const PlatformD3D11&>(platform());
		platform_d3d.BeginScene();

		if(clear)
			platform_d3d.Clear();

		platform_d3d.device_context()->RSSetState(default_render_state_);
		platform_d3d.device_context()->OMSetBlendState(default_blend_state_, NULL, 0xffffffff);
		platform_d3d.device_context()->OMSetDepthStencilState(default_depth_stencil_state_, 0);
	}

	void Renderer3DD3D11::End()
	{
		const PlatformD3D11& platform_d3d = static_cast<const PlatformD3D11&>(platform());
		platform_d3d.EndScene();
	}

	void Renderer3DD3D11::DrawMesh(const class MeshInstance& mesh_instance, bool overwrite_world_matrix)
	{
		const MeshD3D11* mesh = reinterpret_cast<const MeshD3D11*>(mesh_instance.mesh());
		if(mesh != NULL)
		{
			const PlatformD3D11& platform_d3d = static_cast<const PlatformD3D11&>(platform());
			ShaderD3D11* shader_d3d = static_cast<ShaderD3D11*>(shader_);

			if(overwrite_world_matrix)
				set_world_matrix(mesh_instance.transform());

			mesh->SetVertexBuffer(platform_d3d.device_context());

			if(shader_d3d)
			{
				shader_d3d->Set(platform_d3d.device_context());
				shader_d3d->SetConstantBuffers(platform_d3d.device_context(), this);
			}

			for(UInt32 primitive_index=0;primitive_index<mesh->num_primitives();++primitive_index)
			{
				const PrimitiveD3D11* primitive = reinterpret_cast<const PrimitiveD3D11*>(mesh->GetPrimitive(primitive_index));

				// get the texture to use when rendering this primitive
				const TextureD3D11* texture = static_cast<const TextureD3D11*>(primitive->texture());
				if(!texture)
					texture = static_cast<const TextureD3D11*>(default_texture_);

				if(texture)
					texture->Bind(platform_d3d.device_context());

				// Set primitive topology
				platform_d3d.device_context()->IASetPrimitiveTopology( primitive->topology() );

				primitive->SetIndexBuffer(platform_d3d.device_context());

				if(primitive->num_indices() > 0)
					platform_d3d.device_context()->DrawIndexed(primitive->num_indices(), 0, 0);
				else
					platform_d3d.device_context()->Draw(mesh->num_vertices(), 0);
			}
		}
	}

	void Renderer3DD3D11::ClearZBuffer()
	{
	}
}