////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Graphics.h"

Graphics::Graphics()
{
	//initialise objects to null
	m_D3D = 0;
	m_Camera = 0;
	m_Plane = 0;
	m_Model = 0;
	m_FloorModel = 0;
	m_BathModel = 0;
	m_SkyBox = 0;
	m_WaterModel = 0;
	m_FountainModel = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_RenderTexture = 0;
	m_ReflectionShader = 0;
	m_RefractionShader = 0;
	m_WaterShader = 0;
	m_OceanShader = 0;
	m_DebugWindow = 0;
	m_Terrain = 0;
	m_TerrainShader = 0;
	m_ParticleSystem = 0;
	m_ParticleShader = 0;
	m_FireShader = 0;
	m_FogShader = 0;

	Timer = 0;
	
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	D3DXMATRIX baseViewMatrix;

	// Create the camera object.
	m_Camera = new Camera;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 2.0f, -10.0f);
	m_Camera->SetRotation(180.f, 0.0f, 180.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

//////////////////////////////////////////////
//// Models //
//////////////////////////////////////////////

	// Create the Plane object.
	m_Plane = new PlaneClass;
	if(!m_Plane)
	{
		return false;
	}

	// Initialize the plane object.
	result = m_Plane->Initialize(m_D3D->GetDevice(), L"../textures/WaterTex.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the plane object.", L"Error", MB_OK);
		return false;
	}

		// Create the Skybox model object.
	m_SkyBox = new Model;
	if(!m_SkyBox)
	{
		return false;
	}

	// Initialize the skybox model object.
	result = m_SkyBox->Initialize(m_D3D->GetDevice(), "../models/Sphere.obj", L"../textures/skybox.JPG", L"../textures/skybox.JPG");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the floor model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model = new Model;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "../models/Sphere.obj", L"../textures/stone01.DDS", L"../textures/stone01.DDS");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the floor model object.
	m_FloorModel = new RTModel;
	if(!m_FloorModel)
	{
		return false;
	}

	// Initialize the floor model object.
	result = m_FloorModel->Initialize(m_D3D->GetDevice(), "../models/ground.txt", L"../textures/marble01.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the floor model object.", L"Error", MB_OK);
		return false;
	}

	// Create the bath model object.
	m_BathModel = new RTModel;
	if(!m_BathModel)
	{
		return false;
	}

	// Initialize the bath model object.
	result = m_BathModel->Initialize(m_D3D->GetDevice(), "../models/bath.txt", L"../textures/marble01.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bath model object.", L"Error", MB_OK);
		return false;
	}

	// Create the water model object.
	m_WaterModel = new RTModel;
	if(!m_WaterModel)
	{
		return false;
	}

	// Initialize the water model object.
	result = m_WaterModel->Initialize(m_D3D->GetDevice(), "../models/water.txt", L"../textures/water01.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the water model object.", L"Error", MB_OK);
		return false;
	}

	// Create the Terrain model object.
	m_Terrain = new Terrain;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the Terrain model object.
	result = m_Terrain->Initialize(m_D3D->GetDevice(), "../HeightMaps/CourseworkHeightmap.bmp", L"../textures/dirt01.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain model object.", L"Error", MB_OK);
		return false;
	}

	// Create the Fire object
	m_Fire = new FireClass;
	if(!m_Fire)
	{
		return false;
	}

	// Initialize the fire object.
	result = m_Fire->Initialize(m_D3D->GetDevice(), "../models/square.txt", L"../textures/fire01.dds", 
				     L"../textures/noise01.dds", L"../textures/alpha01.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the fire object.", L"Error", MB_OK);
		return false;
	}


///////////////////////////////////////////////////////////////////////////
// Light ect. //
////////////////

	// Create the light shader object.
	m_LightShader = new LightShader;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 0.5f, 0.5f, 1.0f);
	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDirection(5.0f, -1.0f, 0.0f);
	m_Light->SetSpecularColor(5.0f, 0.5f, 0.5f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the Text Object
	m_Text = new Text;
	if(!m_Text)
	{
		return false;
	}
	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
	MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
	return false;
	}

	// Create the bump map object.
	m_BumpMapShader = new BumpMapShader;
	if(!m_BumpMapShader)
	{
		return false;
	}

	// Initialize the Bump Map object.
	result = m_BumpMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
	MessageBox(hwnd, L"Could not initialize the Bump Map object.", L"Error", MB_OK);
	return false;
	}

///////////////////////
// Render to texture //
///////////////////////
	// Create the render to texture object.
	m_RenderTexture = new RenderTexture;
	if(!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTexture;
	if(!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

//////////////////
// Debug Window //
//////////////////
	// Create the debug window object.
	m_DebugWindow = new DebugWindow;
	if(!m_DebugWindow)
	{
		return false;
	}

	// Initialize the debug window object.
	result = m_DebugWindow->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, 100, 100);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

/////////////
// Shaders //
/////////////
	// Create the texture shader object.
	m_TextureShader = new TextureShader;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection shader object.
	m_ReflectionShader = new ReflectionShader;
	if(!m_ReflectionShader)
	{
		return false;
	}

	// Initialize the Reflection shader object.
	result = m_ReflectionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection shader object.", L"Error", MB_OK);
		return false;
	}

		// Create the refraction shader object.
	m_RefractionShader = new RefractionShader;
	if(!m_RefractionShader)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_RefractionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShader;
	if(!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_OceanShader = new OceanShader;
	if(!m_OceanShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_OceanShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the ocean shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShader;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the particle shader object.
	m_ParticleShader = new ParticleShader;
	if(!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the particle system object.
	m_ParticleSystem = new ParticleSystem;
	if(!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(m_D3D->GetDevice(), L"../textures/star.dds");
	if(!result)
	{
		return false;
	}


	// Create the fire shader object.
	m_FireShader = new FireShader;
	if(!m_FireShader)
	{
		return false;
	}

	// Initialize the fire shader object.
	result = m_FireShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the fog shader object.
	m_FogShader = new FogShader;
	if(!m_FogShader)
	{
		return false;
	}

	// Initialize the fog shader object.
	result = m_FogShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the fog shader object.", L"Error", MB_OK);
		return false;
	}


/////////////////
// Water Setup //
/////////////////
	// Set the height of the water.
	m_waterHeight = 1.8f;

	// Initialize the position of the water.
	m_waterTranslation = 0.0f;

	return true;
}

void Graphics::Shutdown()
{
	// Release the fog shader object.
	if(m_FogShader)
	{
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = 0;
	}

	// Release the fire shader object.
	if(m_FireShader)
	{
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	// Release the model object.
	if(m_Fire)
	{
		m_Fire->Shutdown();
		delete m_Fire;
		m_Fire = 0;
	}

	// Release the particle system object.
	if(m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if(m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// Release the water shader object.
	if(m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the refraction shader object.
	if(m_RefractionShader)
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
	}

	// Release the debug window object.
	if(m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// Release the render to texture object.
	if(m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the refraction render to texture object.
	if(m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the reflection shader object.
	if(m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the floor model object.
	if(m_FloorModel)
	{
		m_FloorModel->Shutdown();
		delete m_FloorModel;
		m_FloorModel = 0;
	}

		// Release the water model object.
	if(m_WaterModel)
	{
		m_WaterModel->Shutdown();
		delete m_WaterModel;
		m_WaterModel = 0;
	}

	// Release the bath model object.
	if(m_BathModel)
	{
		m_BathModel->Shutdown();
		delete m_BathModel;
		m_BathModel = 0;
	}

	// Release the bath model object.
	if(m_SkyBox)
	{
		m_SkyBox->Shutdown();
		delete m_SkyBox;
		m_SkyBox = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

		// Release the plane object.
	if(m_Plane)
	{
		m_Plane->Shutdown();
		delete m_Plane;
		m_Plane = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

bool Graphics::Frame(int fps, int cpu, float frameTime, float positionX, float positionY, float positionZ)
{
	bool result;
	static float lightPositionY = -5.0f;
	static float rotation = 0.0f;

		// Update the position of the light each frame.
	lightPositionY += 0.1f;
	if(lightPositionY > 10.0f)
	{
		lightPositionY= -10.0f;
	}

	// Update the position of the light.
	m_Light->SetPosition(lightPositionY, 8.0f, -5.0f);


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the position of the water to simulate motion.
	m_waterTranslation += 0.001f;
	if(m_waterTranslation > 1.0f)
	{
		m_waterTranslation -= 1.0f;
	}

	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(frameTime, m_D3D->GetDeviceContext());

	// Render the graphics scene.
	result = Render(rotation); 
	if(!result)
	{
		return false;
	}

	Timer += 0.04;
	return true;
}


bool Graphics::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	bool result;

////////////////////////////////////////////////////////
// Render Functions //
//////////////////////
	//Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Render the entire scene to the texture first.
	result = RenderToTexture();
	if(!result)
	{
		return false;
	}

	// Render the refraction of the scene to a texture.
	result = RenderRefractionToTexture();
	if(!result)
	{
		return false;
	}

	// Render the scene as normal to the back buffer.
	result = RenderScene();
	if(!result)
	{
		return false;
	}

/////////////////////////////////////////////////////////////////

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	m_D3D->TurnZBufferOff();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);	// required for 2D rendering
	// Do all other rendering first
	m_D3D->TurnOnAlphaBlending();

	// Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_DebugWindow->Render(m_D3D->GetDeviceContext(), 50, 50);
	if(!result)
	{
		return false;
	}

	// Render the debug window using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
					 orthoMatrix, m_RenderTexture->GetShaderResourceView());
	if(!result)
	{
		return false;
	}

	//// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result){return false;}

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();


	// Present the rendered scene to the screen.   
	m_D3D->EndScene();

	return true;
}

bool Graphics::RenderToTexture()
{
	D3DXMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix, viewMatrix, translateMatrix;
	D3DXVECTOR3 scrollSpeeds, scales, cameraPosition, modelPosition;;
	double angle;
	static float rotation = 0.0f;
	bool result;

	// Get the camera reflection view matrix instead of the normal view matrix.
	m_Camera->RenderReflection(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,1,0));

	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.5f, 0.5f, 0.5f, 1.0f);
	
	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();



	// Get the world and projection matrices.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetViewMatrix(viewMatrix);


	m_D3D->TurnZBufferOff();

	D3DXMATRIX tempS; // these Matrix's allow the world matrix to be affected by 2 fors of transformation at the same time.
	D3DXMATRIX tempT;
	
	D3DXMatrixTranslation(&tempT, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	D3DXMatrixScaling(&tempS, 2, 2, 2);
	D3DXMatrixMultiply(&worldMatrix, &tempS, &tempT);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SkyBox->Render(m_D3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_SkyBox->GetIndexCount(), worldMatrix, reflectionViewMatrix, 
				       projectionMatrix, m_SkyBox->GetTextureArray(), m_Light->GetDirection(), 
				       m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}

	
	m_D3D->TurnZBufferOn();


	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	D3DXMATRIX temp;
	
	D3DXMatrixRotationY(&temp, rotation);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &temp);

	D3DXMatrixTranslation(&temp, 7.0f, -9.0f, -5.0f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &temp);


	//renders what i want to have in the reflection/window
	m_Model->Render(m_D3D->GetDeviceContext());

    // Render the model using the bump map shader.
	result = m_BumpMapShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
			 m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());

	if(!result)
	{
		return false;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// particles + BillBoarding //
//////////////////////////////

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Set the position of the model.
	modelPosition.x = 7.0f;
	modelPosition.y = -8.0f;
	modelPosition.z = -5.0f;

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	rotation = (float)angle * 0.0174532925f;

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPosition.x, modelPosition.y, modelPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix); 

///////////////////////////////////////
// Render Particles //
//////////////////////

	m_D3D->TurnOnParticleBlending();

	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, 
					  m_ParticleSystem->GetTexture());
	if(!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	m_D3D->TurnOffAlphaBlending();

	return true;
}

bool Graphics::RenderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix, viewMatrix;
	static float rotation = 0.0f;
	bool result;

	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.5f, 0.5f, 0.5f, 1.0f);
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Translate to where the bath model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 7.0f, 2.0f, -5.0f);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_BathModel->Render(m_D3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_BathModel->GetIndexCount(), worldMatrix, viewMatrix,
					    projectionMatrix, m_BathModel->GetTextureArray(), m_Light->GetDirection(), 
					    m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if(!result)
	{
		return false;
	}

	///////////////////////////////////////////////////////
	// Translate to where the Terrain will be rendered.
	D3DXMatrixTranslation(&worldMatrix, -500.0f, -28.3f, -610.0f);

	// Put the Terrain vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Terrain->Render(m_D3D->GetDeviceContext());

	// Render the Terrain using the Terrain Shader.
	result = m_TerrainShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
					    projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 
						m_Terrain->GetTexture());

	if(!result)
	{
		return false;
	}

	//////////////////////////////////////////////////////

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}


bool Graphics::RenderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionViewMatrix, reflectionMatrix, translateMatrix;
	bool result;
	static float rotation = 0.0f;
	D3DXVECTOR3 scrollSpeeds, scales, cameraPosition, modelPosition;
	D3DXVECTOR2 distortion1, distortion2, distortion3;
	float distortionScale, distortionBias;
	float fogColor, fogStart, fogEnd;
	double angle;
	static float frameTime = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}


///////////////////////////////////////////
// Fire values //
/////////////////

	// Increment the frame time counter.
	frameTime += 0.01f;
	if(frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	// Set the three scrolling speeds for the three different noise textures.
	scrollSpeeds = D3DXVECTOR3(1.3f, 2.1f, 2.3f);

	// Set the three scales which will be used to create the three different noise octave textures.
	scales = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

	// Set the three different x and y distortion factors for the three different noise textures.
	distortion1 = D3DXVECTOR2(0.1f, 0.2f);
	distortion2 = D3DXVECTOR2(0.1f, 0.3f);
	distortion3 = D3DXVECTOR2(0.1f, 0.1f);

	// The the scale and bias of the texture coordinate sampling perturbation.
	distortionScale = 0.8f;
	distortionBias = 0.5f;


//////////////////////////////////////////////
// Fog Values //
////////////////

	// Set the color of the fog to grey.
	fogColor = 0.5f;

	// Set the start and end of the fog.
	fogStart = 1.0f;
	fogEnd = 15.0f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);


////////////////////////////////////////////////////////////
// Skybox //
////////////
	m_D3D->GetWorldMatrix(worldMatrix);

	m_D3D->TurnZBufferOff();

	m_Camera->Render();


	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);


	D3DXMATRIX tempS; // these Matrix's allow the world matrix to be affected by 2 forms of transformation at the same time.
	D3DXMATRIX tempT;
	
	D3DXMatrixTranslation(&tempT, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	D3DXMatrixScaling(&tempS, 2, 2, 2);
	D3DXMatrixMultiply(&worldMatrix, &tempS, &tempT);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SkyBox->Render(m_D3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_SkyBox->GetIndexCount(), worldMatrix, viewMatrix, 
				       projectionMatrix, m_SkyBox->GetTextureArray(), m_Light->GetDirection(), 
				       m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}

	
	m_D3D->TurnZBufferOn();

	m_D3D->GetWorldMatrix(worldMatrix);

//////////////////////////////////////////////
//// Render Models ///
//////////////////////////////////////////////
//Ground //
///////////

	// Translate to where the ground model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 7.0f, 0.2f, -5.0f);

	// Put the floor model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FloorModel->Render(m_D3D->GetDeviceContext());

	// Render the floor model using the reflection shader, reflection texture, and reflection view matrix.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_FloorModel->GetIndexCount(), worldMatrix, viewMatrix,
					    projectionMatrix, m_FloorModel->GetTextureArray(), m_Light->GetDirection(), 
						m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

//////////////////////////////////////////////////////////////////////////////////////
// Bath //
//////////

	// Translate to where the bath model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 7.0f, 1.0f, -5.0f);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_BathModel->Render(m_D3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_BathModel->GetIndexCount(), worldMatrix, viewMatrix, 
				       projectionMatrix, m_BathModel->GetTextureArray(), m_Light->GetDirection(), 
				       m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}
	
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);


///////////////////////////////////////////////////////////////////////
// Sphere //
////////////

	D3DXMATRIX tempR;
//	D3DXMATRIX tempT;
	

	D3DXMatrixRotationY(&tempR, rotation);
	D3DXMatrixTranslation(&tempT, 7.0f, 9.0f, -5.0f);
	D3DXMatrixMultiply(&worldMatrix, &tempR, &tempT);


	//// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	//// Render the model using the bump map shader.
	result = m_BumpMapShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());
	if(!result)
	{
		return false;
	}

	// Get the world matrix again and translate down for the floor model to render underneath the sphere.
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 7.0f, -9.0f, -5.0f); 

	// Get the camera reflection view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	m_D3D->GetWorldMatrix(worldMatrix);


////////////////////////////////////////////////////////////////////////////////////////
// Water ///
////////////
	// Get the camera reflection view matrix.
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	// Translate to where the water model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 7.0f, m_waterHeight, -5.0f); 

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WaterModel->Render(m_D3D->GetDeviceContext());

	// Render the water model using the water shader.
	result = m_WaterShader->Render(m_D3D->GetDeviceContext(), m_WaterModel->GetIndexCount(), worldMatrix, viewMatrix, 
				       projectionMatrix, reflectionMatrix, m_RenderTexture->GetShaderResourceView(),
				       m_RefractionTexture->GetShaderResourceView(), m_WaterModel->GetTextureArray(), 
				       m_waterTranslation, 0.1f);
	if(!result)
	{
		return false;
	}

	D3DXMatrixTranslation(&worldMatrix, -6.0f, 6.0f, -20.0f);

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the water model using the water shader.
	result = m_WaterShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, 
				       projectionMatrix, reflectionMatrix, m_RenderTexture->GetShaderResourceView(),
				       m_RefractionTexture->GetShaderResourceView(), m_WaterModel->GetTextureArray(), 
				       m_waterTranslation, 0.1f);
	if(!result)
	{
		return false;
	}



	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

/////////////////////////////////////////////////////////////////////////////
// Terrain //
/////////////

	// Translate to where the Terrain will be rendered.
	D3DXMatrixTranslation(&worldMatrix, -500.0f, -28.3f, -610.0f);

	// Put the Terrain vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Terrain->Render(m_D3D->GetDeviceContext());

	// Render the Terrain using the Terrain Shader.
	result = m_TerrainShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
					    projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 
						m_Terrain->GetTexture());

	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

/////////////////////////////////////////////////////////////////////////////////////////
// Ocean Shader///
//////////////////

	m_D3D->TurnOnAlphaBlending();

	// Get the camera reflection view matrix.
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	// Translate to where the water model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, -500.0f, -9.1f, -610.0f); 

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Plane->Render(m_D3D->GetDeviceContext());

	// Render the water model using the water shader. m_OceanModel
	result = m_OceanShader->Render(m_D3D->GetDeviceContext(), m_Plane->GetIndexCount(), worldMatrix, viewMatrix, 
				       projectionMatrix, m_Plane->GetTextureArray(), m_Light->GetDirection(), 
				       m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), Timer);
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	m_D3D->TurnOffAlphaBlending();


	// Translate to where the water model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 13.0f, 4.0f, -12.0f); 

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the water model using the water shader. m_OceanModel
	result = m_OceanShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, 
				       projectionMatrix, m_Model->GetTextureArray(), m_Light->GetDirection(), 
				       m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), Timer);
	if(!result)
	{
		return false;
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// particles + BillBoarding //
//////////////////////////////

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Set the position of the model.
	modelPosition.x = 7.0f;
	modelPosition.y = 2.0f;
	modelPosition.z = -5.0f;

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	rotation = (float)angle * 0.0174532925f;

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPosition.x, modelPosition.y, modelPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix); 

///////////////////////////////////////
// Render Particles //
//////////////////////

	m_D3D->TurnOnParticleBlending();

	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
					  m_ParticleSystem->GetTexture());
	if(!result)
	{
		return false;
	}

	m_D3D->TurnOnAlphaBlending();

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Fire + BillBoarding //
/////////////////////////

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Set the position of the model.
	modelPosition.x = -12.0f;
	modelPosition.y = 4.0f;
	modelPosition.z = -9.0f;

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	rotation = (float)angle * 0.0174532925f;

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&worldMatrix, rotation + 5);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPosition.x, modelPosition.y, modelPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix); 

/////////////////////////////////////////
// Render Fire //
/////////////////

	// Put the square model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the square model using the fire shader.
	result = m_FireShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								  m_Fire->GetTexture1(), m_Fire->GetTexture2(), m_Fire->GetTexture3(), frameTime, scrollSpeeds, 
								  scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending.
	m_D3D->TurnOffAlphaBlending();


	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fog ////////
///////////////

	D3DXMatrixTranslation(&worldMatrix, -6.0f, 6.0f, 0.0f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model with the fog shader.
	result = m_FogShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				     m_Model->GetTextureArray(), fogStart, fogEnd);
	if(!result)
	{
		return false;
	}

	return true;
}

////////////////////////
// Movement Functions //
////////////////////////
void Graphics::frwd()
{
	m_Camera-> MoveFWRD();
}

void Graphics::bkwrd()
{
	m_Camera-> MoveBKWRD();
}

void Graphics::left()
{
	m_Camera-> MoveLEFT();
}

void Graphics::right()
{
	m_Camera-> MoveRIGHT();
}

void Graphics::up()
{
	m_Camera-> MoveUP();
}

void Graphics::down()
{
	m_Camera-> MoveDOWN();
}

void Graphics::rotL()
{
	m_Camera-> RotateLEFT();
}

void Graphics::rotR()
{
	m_Camera-> RotateRIGHT();
}

void Graphics::rotD()
{
	m_Camera-> RotateDOWN();
}

void Graphics::rotU()
{
	m_Camera-> RotateUP();
}