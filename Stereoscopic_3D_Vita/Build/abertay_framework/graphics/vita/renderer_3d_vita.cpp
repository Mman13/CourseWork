#include <graphics/vita/renderer_3d_vita.h>
#include <system/vita/platform_vita.h>
#include <graphics/vita/default_3d_shader_vita.h>
#include <graphics/vita/skinned_mesh_shader_vita.h>
#include <graphics/vita/texture_vita.h>
#include <graphics/vita/mesh_vita.h>
#include <graphics/vita/primitive_vita.h>
#include <graphics/mesh_instance.h>
#include <graphics/colour.h>
#include <gxm.h>

namespace abfw
{
	class Platform;

	Renderer3DVita::Renderer3DVita(const Platform& platform) :
		Renderer3D(platform)
	{
		PointLight default_point_light;
		default_point_light.set_colour(Colour(1.0f, 1.0f, 1.0f, 1.0f));
		default_point_light.set_position(Vector3(0.0f, 1000.0f, 1000.0f));
		default_shader_data_.AddPointLight(default_point_light);
		//default_shader_data_.set_ambient_light_colour(Colour(1.0f, 0.0f, 0.0f, 1.0f));

		const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform);
		default_shader_ = new Default3DShaderVita(platform_vita.shader_patcher(), &default_shader_data_);
		shader_ = default_shader_;

		default_texture_ = static_cast<TextureVita*>(Texture::CreateCheckerTexture(64, 8, platform));

		default_skinned_mesh_shader_ = new SkinnedMeshShaderVita(platform_vita.shader_patcher(), &default_skinned_mesh_shader_data_);
	}

	Renderer3DVita::~Renderer3DVita()
	{
		CleanUp();
	}

	void Renderer3DVita::CleanUp()
	{
		DeleteNull(default_texture_);
		DeleteNull(default_shader_);
	}


	void Renderer3DVita::Begin(bool clear)
	{
		const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform());
		platform_vita.BeginScene();

		if(clear)
			platform_vita.Clear();
	}

	void Renderer3DVita::End()
	{
		const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform());
		platform_vita.EndScene();
	}

	void Renderer3DVita::DrawMesh(const class MeshInstance& mesh_instance, bool overwrite_world_matrix)
	{
		const MeshVita* mesh = reinterpret_cast<const MeshVita*>(mesh_instance.mesh());
		if(mesh != NULL)
		{
			const PlatformVita& platform_vita = static_cast<const PlatformVita&>(platform());
			ShaderVita* shader_vita = static_cast<ShaderVita*>(shader_);

			SceGxmContext* context = platform_vita.context();

			if(overwrite_world_matrix)
				set_world_matrix(mesh_instance.transform());

			mesh->SetVertexBuffer(platform_vita.context());

			if(shader_vita)
			{
				shader_vita->Set(platform_vita.context());
				shader_vita->SetConstantBuffers(platform_vita.context(), this);
			}

			SceGxmErrorCode error_code = SCE_OK;
			for(UInt32 primitive_index=0;primitive_index<mesh->num_primitives();++primitive_index)
			{
				const PrimitiveVita* primitive = reinterpret_cast<const PrimitiveVita*>(mesh->GetPrimitive(primitive_index));

				// get the texture to use when rendering this primitive
				const TextureVita* texture = static_cast<const TextureVita*>(primitive->texture());
				if(!texture)
					texture = static_cast<const TextureVita*>(default_texture_);

				if(texture)
				{
					sceGxmTextureSetUAddrMode(const_cast<SceGxmTexture*>(&texture->texture()), SCE_GXM_TEXTURE_ADDR_REPEAT);
					sceGxmTextureSetVAddrMode(const_cast<SceGxmTexture*>(&texture->texture()), SCE_GXM_TEXTURE_ADDR_REPEAT);

					sceGxmSetFragmentTexture(platform_vita.context(), 0, const_cast<SceGxmTexture*>(&texture->texture()));
				}

				error_code = sceGxmDraw(context, primitive->topology(), primitive->index_format(), primitive->indices(), primitive->num_indices());
			}
		}
	}

	void Renderer3DVita::ClearZBuffer()
	{
	}
}