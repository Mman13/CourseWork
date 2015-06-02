#include <graphics/renderer_3d.h>
#include <graphics/shader.h>

namespace abfw
{
	Renderer3D::Renderer3D(const Platform& platform) :
		platform_(platform),
		shader_(NULL),
		default_shader_(NULL),
		default_skinned_mesh_shader_(NULL)
	{
	}

	Renderer3D::~Renderer3D()
	{
		delete default_shader_;
		delete default_skinned_mesh_shader_;
	}

	void Renderer3D::SetShader( Shader* shader)
	{
		if(shader == NULL)
			set_shader(default_shader_);
		else
			set_shader(shader);
	}

	void Renderer3D::DrawSkinnedMesh(const MeshInstance& mesh_instance, const std::vector<Matrix44>& bone_matrices, bool overwrite_world_matrix, bool use_default_shader)
	{
		Shader* previous_shader = shader_;
		if(use_default_shader)
		{
			// copy lighting from default shader data
			// GRC FIXME need to separate light data out
			// so we don't need to do this
			default_skinned_mesh_shader_data_.CleanUp();

			for(Int32 light_num=0;light_num<default_shader_data_.GetNumPointLights();++light_num)
			{
				default_skinned_mesh_shader_data_.AddPointLight(default_shader_data_.GetPointLight(light_num));
			}
			default_skinned_mesh_shader_data_.set_ambient_light_colour(default_shader_data_.ambient_light_colour());

			default_skinned_mesh_shader_data_.set_bone_matrices(&bone_matrices);


			SetShader(default_skinned_mesh_shader_);
		}

		DrawMesh(mesh_instance, overwrite_world_matrix);

		if(use_default_shader)
			SetShader(previous_shader);
	}

	void Renderer3D::CalculateInverseWorldTransposeMatrix()
	{
		Matrix44 inv_world;
		inv_world.Inverse(world_matrix_);
		inv_world_transpose_matrix_.Transpose(inv_world);
	}

	void Renderer3D::set_world_matrix(const  Matrix44& matrix)
	{
		world_matrix_ = matrix;
		CalculateInverseWorldTransposeMatrix();
	}
}

