// s3d.cpp
// Author Grant Clark
// Edited 27/2/15 by Michael Little - 1103677  

//includes
#include "s3d.h"

#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <assets/obj_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <maths/vector2.h>
#include <input/sony_controller_input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>

#include "d3d11/left_eye_3d_shader.h"
#include "d3d11/right_eye_3d_shader.h"
#include "d3d11/s3d_shader_d3d11.h"

#include <system/d3d11/platform_d3d11.h>
#include <graphics/d3d11/renderer_3d_d3d11.h>

S3D::S3D(abfw::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	left_eye_shader_(NULL),
	right_eye_shader_(NULL),
	s3d_shader_(NULL)
{
}

void S3D::Init()
{
	sprite_renderer_ = platform_.CreateSpriteRenderer();
	renderer_3d_ = platform_.CreateRenderer3D();

	controller_input_manager_ = platform_.CreateSonyControllerInputManager();

	InitFont();

	abfw::OBJLoader obj_loader; //sets up the object loader
	obj_loader.Load("ball1.obj", platform_, model_); // loads in a model
	 tarScale = abfw::Vector3(0.05f, 0.05f, 0.05f); //scale ud like the model to be
	 obScale = abfw::Vector3(20.0f, 20.0f, 20.0f);

	mesh_ = CreateCubeMesh(); //creates a cube mesh

	//initialises bool's for colision
	collision_ = false;
	hit_ = false;

	m_System.SetMinObstacle(0);
	m_System.SetObstacleSpeed(2);
	m_System.SetTargetSpeed(2.5);
	m_System.SetScore(0);
	m_System.SetTimer(30);
	m_System.SetCollided(false);
	m_System.SetGameOver(false);
	m_System.SetPaused(false);
	m_System.SetStarted(false);

	for(unsigned i = 0; i < 60; i++) //initialise the targets within this loop
	{
		target.push_back(GameObject()); //adds one target to the array
		target[i].set_mesh(model_.mesh()); //sets the target mesh
		target[i].SetScale(tarScale); //sets the scale
		target[i].SetRadius(3);
	}

	
	for(unsigned i = 0; i < 1; i++) //initialise the obstacles within this loop
	{
		obstacle.push_back(GameObject()); //adds one obstacle to the list
		obstacle[i].set_mesh(mesh_); // sets the Obstacle mesh
		obstacle[i].SetScale(obScale); //sets the scale
		obstacle[i].SetObstacle(true); //sets the GameObject variable obstacle_ to true
		obstacle[i].SetRadius(50); //sets the radius of the bounding sphere
		obstacle[i].SetPosition(abfw::Vector3(-80 + rand()%140, -80 + rand()%160, -1200 - rand()%800));
		obstacle[i].SetObjectSpeed(m_System.GetObstacleSpeed());
					
	}

	// initialise the camera settings
	m_Camera.SetRotation(abfw::Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
	m_Camera.SetCameraEye(abfw::Vector3(0.0f, 0.0f, 0.0f));
	m_Camera.SetCameraLookat(abfw::Vector3(0.0f, 0.0f, 0.0f));
	m_Camera.SetCameraUp(abfw::Vector3(0.0f, 1.0f, 0.0f));
	m_Camera.SetNearPlane(0.01f);
	m_Camera.SetFarPlane(1000.f);
	m_Camera.SetFOV(abfw::DegToRad(45.0f));
	m_Camera.SetEyeSeperationDistance(0.032f);
	m_Camera.SetConvergenceDistance(1.75f);
	m_Camera.SetRadius(2.0f);
	m_Camera.SetSetup(false);
	m_Camera.SetCameraLookatLeft(abfw::Vector3(0.0f, 0.0f, -m_Camera.GetEyeSeperationDistance() * 0.5));
	m_Camera.SetCameraLookatRight(abfw::Vector3(0.0f, 0.0f, m_Camera.GetEyeSeperationDistance() * 0.5));
	m_Camera.Render();

	// set up render targets for each eye
	left_eye_render_target_ = platform_.CreateRenderTarget();
	right_eye_render_target_ = platform_.CreateRenderTarget();

	s3d_sprite_.set_position(abfw::Vector3(platform_.width()*0.5f, platform_.height()*0.5f, 0.5f));
	s3d_sprite_.set_width(platform_.width());
	s3d_sprite_.set_height(platform_.height());


	// create the shaders for rendering the scene for the different eyes
	// and the shader that combines both render targets to produce the final image
	abfw::PlatformD3D11& platform_d3d11 = static_cast<abfw::PlatformD3D11&>(platform_);
	left_eye_shader_ = new LeftEye3DShader(platform_d3d11.device(), renderer_3d_->default_shader_data());
	right_eye_shader_ = new RightEye3DShader(platform_d3d11.device(), renderer_3d_->default_shader_data());
	s3d_shader_ = new S3DShaderD3D11(platform_d3d11.device());
	s3d_shader_->set_left_eye_texture(left_eye_render_target_->texture());
	s3d_shader_->set_right_eye_texture(right_eye_render_target_->texture());

	// clear the screen to black
	// screen must be black for S3D to work
	platform_.set_render_target_clear_colour(abfw::Colour(0.0f, 0.0f, 0.0f, 1.f));
}

void S3D::CleanUp()
{
	delete s3d_shader_;
	s3d_shader_ = NULL;

	delete left_eye_shader_;
	left_eye_shader_ = NULL;

	delete right_eye_shader_;
	right_eye_shader_ = NULL;

	delete left_eye_render_target_;
	left_eye_render_target_ = NULL;

	delete right_eye_render_target_;
	right_eye_render_target_ = NULL;

	delete mesh_;
	mesh_ = NULL;
	
	CleanUpFont();
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	model_.Release();
}

bool S3D::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	
	m_System.Update(); //updates the game logic (e.g. collision)

	if(m_System.GetGameOver() == false) // as long as GameOver_ is false the contents of the loop will run
	{
		if(m_System.GetPaused() == false)
		{

			m_Camera.Update(); //updates the player camera


			if(m_System.GetTimer() > 0) // timer
			{
				m_System.ChangeTimer(-0.02);
			}

			for(int t = 0; t < target.size(); t++) //updates the targets and their collision
			{
				target[t].SetObjectSpeed(m_System.GetTargetSpeed());
				target[t].Update();
				
				if(target.size() < 50)
				{
					for(unsigned i = 0; i < 1; i++) // replenishes target
					{
						target.push_back(GameObject());
						target[i].set_mesh(model_.mesh()); //sets the target mesh
						target[i].SetScale(tarScale); //sets the scale
						target[i].SetRadius(3);
					}
				}

				if( m_System.SphereCollision(&target[t], &m_Camera) == true) //target/camera collision
				{
					if (collision_ == false)
					{
						std::cout << "collision" << std::endl;
						collision_ = true;
					}
				}
				else
				{
					std::cout << "no collision" << std::endl;
					m_System.SetCollided(false);
				}
		
				if(collision_ == true) //if camera / target collision happened
				{
					if(m_System.GetCollided() == false) //makes sure collision doesnt spam
					{
						m_System.ChangeScore(1); // adds to score
						m_System.ChangeTimer(3); // increases timer
						target[t] = target[target.size() - 1];
						target.pop_back(); // removes the hit target
						m_System.SetCollided(true);
						collision_ = false;
					}

					for(unsigned i = 0; i < 2; i++) // replenishes target
					{
						target.push_back(GameObject());
						target[i].set_mesh(model_.mesh()); //sets the target mesh
						target[i].SetScale(tarScale); //sets the scale
						target[i].SetRadius(3);
					}

				}
			}

			for(int ob = 0; ob < obstacle.size(); ob++) //updates the obstacles and their collision
			{
				obstacle[ob].SetObjectSpeed(m_System.GetObstacleSpeed());
				obstacle[ob].Update();	

				if(obstacle[ob].GetPosition().z >= 60)
				{
					//obstacle[ob].SetPosition(abfw::Vector3(-80 + rand()%140, -80 + rand()%160, -1000 - rand()%800));
					obstacle[ob] = obstacle[obstacle.size() - 1];
					obstacle.pop_back();
				}
				
				if( m_System.SphereCollision(&obstacle[ob], &m_Camera) == true)//camera / obstacle collision
				{
					if (hit_ == false)
					{
						std::cout << "obstacle collision" << std::endl;
						hit_ = true;
					}
				}
				else
				{
					std::cout << "no obstacle collision" << std::endl;
					m_System.SetCollided(false);
					hit_ = false;
				}
		
				if(hit_ == true)
				{
					if(m_System.GetCollided() == false)
					{
						obstacle[ob] = obstacle[obstacle.size() - 1];
						obstacle.pop_back(); // removes hit obstacle
						m_System.SetGameOver(true);
						m_System.SetCollided(true);
						hit_ = false;
					}

				}

				if(obstacle.size() < m_System.GetMinObstacle())
				{
					for(unsigned int i = 0; i < m_System.GetMinObstacle(); i++) //handels obstacle count
					{
						obstacle.push_back(GameObject());
						obstacle[i].set_mesh(mesh_);
						obstacle[i].SetScale(obScale);
						obstacle[i].SetObstacle(true);
						obstacle[i].SetRadius(50);
						obstacle[i].SetPosition(abfw::Vector3(-80 + rand()%140, -80 + rand()%160, -1200 - rand()%800));
						obstacle[i].SetObjectSpeed(m_System.GetObstacleSpeed());
					}
				}
			}
	

			if(controller_input_manager_) // vita controls
			{
				controller_input_manager_->Update();
				const abfw::SonyController* controller = controller_input_manager_->GetController(0);
				if(controller)
				{
					if(controller->buttons_pressed() & ABFW_SONY_CTRL_UP )
					{
						m_Camera.MoveUP();
					}

					if(controller->buttons_pressed() & ABFW_SONY_CTRL_DOWN )
					{
						m_Camera.MoveDOWN();
					}

					if(controller->buttons_pressed() & ABFW_SONY_CTRL_LEFT )
					{
						m_Camera.MoveLEFT();
					}

					if(controller->buttons_pressed() & ABFW_SONY_CTRL_RIGHT )
					{
						m_Camera.MoveRIGHT();
					}

					if(controller->buttons_pressed() & ABFW_SONY_CTRL_CROSS )
					{
				
					}

					if(controller->buttons_pressed() & ABFW_SONY_CTRL_L1)
					{
						//player_.transform().SetTranslation(abfw::Vector3(+1.0f, 0.0f, 0.0f));
					}
				}
			}

			//keyboard controls
			if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			{
				// do something when the left arrow is pressed
				m_Camera.MoveLEFT();
			}

			if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			{
				// do something when the right arrow is pressed
				m_Camera.MoveRIGHT();
			}

			if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			{
				// do something when the up arrow is pressed
				m_Camera.MoveUP();
			}

			if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			{
				// do something when the down arrow is pressed
				m_Camera.MoveDOWN();
			}

			if( ::GetAsyncKeyState(VK_SPACE) & 0x8000f )
			{
				// do something when space is pressed
				m_System.SetPaused(true);
			}

			// exit application if esc is pressed
			bool running = true;
			if( ::GetAsyncKeyState(VK_ESCAPE) & 0x8000f )
				running = false;

			return running;
		}

		if(m_System.GetPaused() == true)
		{

			if( ::GetAsyncKeyState(VK_SPACE) & 0x8000f )
			{
					// do something when space is pressed
				m_System.SetPaused(false);
			}
			if( ::GetAsyncKeyState(VK_SHIFT) & 0x8000f )
			{
				// do something when shift is pressed
				m_Camera.UpdateConDis(0.01);
			}

			if( ::GetAsyncKeyState(VK_CONTROL) & 0x8000f )
			{
				// do something when shift is pressed
				m_Camera.UpdateConDis(-0.01);
			}
			// exit application if esc is pressed
			bool running = true;
			if( ::GetAsyncKeyState(VK_ESCAPE) & 0x8000f )
				running = false;

			return running;
		}

		
	}
}

void S3D::Render()
{
	m_Camera.Render(); //updates the camera variables for 3D

	// calculate projection and view matrices
	abfw::Matrix44 projection_matrix;
	abfw::Matrix44 left_projection_matrix;
	abfw::Matrix44 right_projection_matrix;
	
	// sets up 3d
	projection_matrix = platform_.PerspectiveProjectionFov(m_Camera.GetFOV(), (float)platform_.width()/ (float)platform_.height(), m_Camera.GetNearPlane(), m_Camera.GetFarPlane());

	aspect_ratio = (float)platform_.width() / (float)platform_.height();

	height = tan(m_Camera.GetFOV() / 2) * m_Camera.GetNearPlane();
	a = tan(m_Camera.GetFOV() / 2)* aspect_ratio * m_Camera.GetConvergenceDistance();
	b = a - m_Camera.GetEyeSeperationDistance() * 0.5;
	c = a + m_Camera.GetEyeSeperationDistance() * 0.5;
	Dleft = m_Camera.GetNearPlane() * b / m_Camera.GetConvergenceDistance();
	Dright = m_Camera.GetNearPlane() * c / m_Camera.GetConvergenceDistance();

	left_projection_matrix = platform_.PerspectiveProjectionFrustum(-Dleft, Dright, height, -height, m_Camera.GetNearPlane(), m_Camera.GetFarPlane());
	right_projection_matrix = platform_.PerspectiveProjectionFrustum(-Dright, Dleft, height, -height, m_Camera.GetNearPlane(), m_Camera.GetFarPlane());

	// render the scene once for each eye
	RenderScene(left_eye_render_target_, left_eye_shader_, m_Camera.GetLeftViewMatrix(), left_projection_matrix);	
	RenderScene(right_eye_render_target_, right_eye_shader_, m_Camera.GetRightViewMatrix(), right_projection_matrix);
	
	// set the render target back to the default render target
	platform_.set_render_target(NULL);

	// clear the render target before drawing anything
	// by passing true into Begin
	sprite_renderer_->Begin(true);

	// set the sprite shader back to the default sprite shader
	sprite_renderer_->SetShader(NULL);

	// draw final S3D image
	sprite_renderer_->SetShader(s3d_shader_);
	sprite_renderer_->DrawSprite(s3d_sprite_);

	// set the sprite shader back to the default sprite shader
	sprite_renderer_->SetShader(NULL);
	DrawHUD();
	sprite_renderer_->End();
}

void S3D::RenderScene(abfw::RenderTarget* render_target, abfw::Shader* shader, const abfw::Matrix44& view_matrix, const abfw::Matrix44& projection_matrix)
{
	platform_.set_render_target(render_target); //sets where application will be rendered
	//sets matrix's for 3D rendering
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);
	renderer_3d_->SetShader(shader);

	// clear the render target before drawing anything
	// by passing true into Begin
	renderer_3d_->Begin(true);

	for(int t = 0; t < target.size(); t++)//renders targets
	{
		renderer_3d_->DrawMesh(target[t]);
	}

	for(int o = 0; o < obstacle.size(); o++) // renders obstacles
	{
		renderer_3d_->DrawMesh(obstacle[o]);
	}
	
	renderer_3d_->End();
}


void S3D::InitFont() //initialise fonts here
{
	font_ = new abfw::Font();
	font_->Load("comic_sans", platform_);
}

void S3D::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void S3D::DrawHUD()
{
	if(font_) // include stuff to be draw on screen here
	{
		font_->RenderText(sprite_renderer_, abfw::Vector3(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, abfw::TJ_LEFT, "FPS: %.1f", fps_);
		font_->RenderText(sprite_renderer_, abfw::Vector3(10.0f, 10.0f, -0.8f), 1.0f, 0xffffffff, abfw::TJ_LEFT, "Time remaining: %.0f", m_System.GetTimer());
		font_->RenderText(sprite_renderer_, abfw::Vector3(10.0f, 30.0f, -0.7f), 1.0f, 0xffffffff, abfw::TJ_LEFT, "Score: %.0f", m_System.GetScore());

		if(m_System.GetGameOver() == true)
		{
			font_->RenderText(sprite_renderer_, abfw::Vector3(400.0f, 100.0f, -0.5f), 1.0f, 0xffffffff, abfw::TJ_LEFT, "GameOver! Final Score = %.0f", m_System.GetScore());
		}

		if(m_System.GetPaused() == true)
		{
			font_->RenderText(sprite_renderer_, abfw::Vector3(400.0f, 150.0f, -0.6f), 1.0f, 0xffffffff, abfw::TJ_LEFT, "Paused", m_System.GetScore());
		}
	}
}

abfw::Mesh* S3D::CreateCubeMesh() //creates a cube when called
{
	abfw::Mesh* mesh = platform_.CreateMesh();

	int scale = 2.1; //used to set the cubes size

	const abfw::Mesh::Vertex vertices[] = {
		{abfw::Vector3(scale, -scale, -scale), abfw::Vector3(0.577f, -0.577f, -0.577f), abfw::Vector2(0.0f, 0.0f)},
		{abfw::Vector3(scale, scale, -scale), abfw::Vector3(0.577f, 0.577f, -0.577f), abfw::Vector2(0.0f, 0.0f)},
		{abfw::Vector3(-scale, scale, -scale), abfw::Vector3(-0.577f, 0.577f, -0.577f), abfw::Vector2(0.0f, 0.0f)},
		{abfw::Vector3(-scale, -scale, -scale), abfw::Vector3(-0.577f, -0.577f, -0.577f), abfw::Vector2(0.0f, 0.0f)},
		{abfw::Vector3(scale, -scale, scale), abfw::Vector3(0.577f, -0.577f, 0.577f), abfw::Vector2(0.0f, 0.0f)},
		{abfw::Vector3(scale, scale, scale), abfw::Vector3(0.577f, 0.577f, 0.577f), abfw::Vector2(0.0f, 0.0f)},
		{abfw::Vector3(-scale, scale, scale), abfw::Vector3(-0.577f, 0.577f, 0.577f), abfw::Vector2(0.0f, 0.0f)},
		{abfw::Vector3(-scale, -scale, scale), abfw::Vector3(-0.577f, -0.577f, 0.577f), abfw::Vector2(0.0f, 0.0f)}	};



	abfw::Aabb aabb(abfw::Vector3(-scale, -scale, -scale), abfw::Vector3(scale, scale, scale));
	mesh->set_aabb(aabb);

	abfw::Sphere sphere(aabb);
	mesh->set_bounding_sphere(sphere);

	mesh->InitVertexBuffer(platform_, static_cast<const void*>(vertices), sizeof(vertices)/sizeof(abfw::Mesh::Vertex), sizeof(abfw::Mesh::Vertex));

	mesh->AllocatePrimitives(1);
	abfw::Primitive* primitive = mesh->GetPrimitive(0);

	const UInt32 indices[] = {
    // Back
    0, 1, 2,
    2, 3, 0,
    // Front
    6, 5, 4,
    4, 7, 6,
    // Left
    2, 7, 3,
    2, 6, 7,
    // Right
    0, 4, 1,
	5, 1, 4,
    // Top
    6, 2, 1, 
	5, 6, 1,

	// Bottom
    0, 3, 7,
    0, 7, 4 
	};

	primitive->InitIndexBuffer(platform_, static_cast<const void*>(indices), sizeof(indices)/sizeof(UInt32),sizeof(UInt32));
	primitive->SetType(abfw::TRIANGLE_LIST);
	return mesh;
}


