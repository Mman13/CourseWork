// s3d.h
// Author Grant Clark
// Edited 27/2/15 by Michael Little - 1103677  

#ifndef S3D_H
#define S3D_H

//includes
#include "GameObject.h"
#include "Camera.h"
#include "System.h"

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector3.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <graphics/model.h>
#include <iostream>

// FRAMEWORK FORWARD DECLARATIONS
namespace abfw
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class RenderTarget;
	class Shader;
	class SonyControllerInputManager;
}

//shaders for 3D
class LeftEye3DShader;
class RightEye3DShader;
class S3DShaderD3D11;

class S3D : public abfw::Application
{
public:
	S3D(abfw::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

private:
	std:: vector <GameObject> target; // array of targets
	std:: vector <GameObject> obstacle; // array of obstacles

	abfw::Mesh* CreateCubeMesh(); // function that creates a cube mesh when called.
	abfw::Model model_; // used for loaded model

	Camera m_Camera; // declares an instance of the camera class

	System m_System; // declares an instance of the System class

	void InitFont(); // used to initialise the font's for text.
	void CleanUpFont();
	void DrawHUD(); // used to draw text on screen

	abfw::SonyControllerInputManager* controller_input_manager_;

	void RenderScene(abfw::RenderTarget* render_target, abfw::Shader* shader, const abfw::Matrix44& view_matrix, const abfw::Matrix44& projection_matrix);

	abfw::SpriteRenderer* sprite_renderer_;
	abfw::Font* font_;

	float fps_;

	class abfw::Renderer3D* renderer_3d_;
	class abfw::Mesh* mesh_;
	abfw::MeshInstance cube_player_;

///////////////////////////////////////
// used to create 3D
	float aspect_ratio;
	float height;
	float a;
	float b;
	float c;
	float Dleft;
	float Dright;

///////////////////////////////////////
// bools used for colision detection
	bool collision_;
	bool hit_;

	abfw::Vector3 obScale; //used to scale the obstacles
	abfw::Vector3 tarScale; //used to scale the target

///////////////////////////////////////
//declares varaibles used for displaying for the left and right eye.
	abfw::RenderTarget* left_eye_render_target_;
	abfw::RenderTarget* right_eye_render_target_;
	abfw::Sprite left_eye_sprite_;
	abfw::Sprite right_eye_sprite_;
	abfw::Sprite s3d_sprite_;

	LeftEye3DShader* left_eye_shader_;
	RightEye3DShader* right_eye_shader_;
	S3DShaderD3D11* s3d_shader_;
};

#endif // S3D_H