#ifndef _ABFW_RENDERER_3D_H
#define _ABFW_RENDERER_3D_H

#include <abertay_framework.h>
#include <maths/matrix44.h>
#include <graphics/default_3d_shader_data.h>
#include <graphics/skinned_mesh_shader_data.h>
#include <vector>

namespace abfw
{
	class Platform;
	class MeshInstance;
	class Shader;

	class Skeleton;


	class Renderer3D
	{
	public:
		Renderer3D(const Platform& platform);
		virtual ~Renderer3D();
	//	virtual void ClearZBuffer() = 0;
		virtual void Begin(bool clear = true) = 0;
		virtual void End() = 0;
		virtual void DrawMesh(const  MeshInstance& mesh_instance, bool overwrite_world_matrix = true) = 0;
		void DrawSkinnedMesh(const  MeshInstance& mesh_instance, const std::vector<Matrix44>& bone_matrices, bool overwrite_world_matrix = true, bool use_default_shader = true);
		void SetShader( Shader* shader);

		inline  Shader* shader() const { return shader_; }
		inline const Matrix44& view_matrix() const { return view_matrix_; }
		inline void set_view_matrix(const  Matrix44& matrix) {view_matrix_ = matrix;}
		inline const Matrix44& projection_matrix() const { return projection_matrix_; }
		inline void set_projection_matrix(const  Matrix44& matrix) {projection_matrix_ = matrix;}
		inline const Matrix44& world_matrix() const { return world_matrix_; }
		void set_world_matrix(const  Matrix44& matrix);
		inline const Matrix44& inv_world_transpose_matrix() const { return inv_world_transpose_matrix_; }

		inline  const Platform& platform() const {return platform_;}
		inline Default3DShaderData& default_shader_data() { return default_shader_data_; }
	protected:
		void CalculateInverseWorldTransposeMatrix();
		inline void set_shader( Shader* shader) { shader_ = shader; }

		Matrix44 projection_matrix_;
		Matrix44 view_matrix_;
		Matrix44 inv_world_transpose_matrix_;
		Shader* shader_;
		Shader* default_shader_;
		Shader* default_skinned_mesh_shader_;
		Default3DShaderData default_shader_data_;
		SkinnedMeshShaderData default_skinned_mesh_shader_data_;
	private:
		const Platform& platform_;
		Matrix44 world_matrix_;
	};
}
#endif // _ABFW_RENDERER_3D_H