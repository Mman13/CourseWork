////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

//////////////
// INCLUDES //
//////////////
#include <windows.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3D.h"
#include "Camera.h"
#include "PlaneClass.h"
#include "Model.h"
#include "RTmodel.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "BumpMapShader.h"
#include "LightClass.h"
#include "TokenStream.h"
#include "FontShader.h"
#include "Font.h"
#include "Text.h"
#include "RenderTexture.h"
#include "DebugWindow.h"
#include "ReflectionShader.h"
#include "RefractionShader.h"
#include "WaterShader.h"
#include "OceanShader.h"
#include "Terrain.h"
#include "TerrainShader.h"
#include "ParticleSystem.h"
#include "ParticleShader.h"
#include "FireClass.h"
#include "FireShader.h"
#include "FogShader.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////////////
// Class name: GraphicsClass //
///////////////////////////////
class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float, float, float, float);
	bool Render(float rotation);
	float Timer;

	// Movement Functions
	void frwd();
	void bkwrd();
	void left();
	void right();
	void up();
	void down();
	void rotL();
	void rotR();
	void rotD();
	void rotU();

private:
	bool RenderToTexture();
	bool RenderRefractionToTexture();
	bool RenderScene();
	
private:
	D3DClass* m_D3D;
	Camera* m_Camera;
	PlaneClass* m_Plane;
	Model *m_Model, *m_FountainModel, *m_SkyBox;
	RTModel *m_FloorModel, *m_BathModel, *m_WaterModel; //RasterTek models 
	TextureShader* m_TextureShader;
	RenderTexture *m_RenderTexture, *m_RefractionTexture;
	ReflectionShader* m_ReflectionShader;
	DebugWindow* m_DebugWindow;
	LightClass* m_Light;
	LightShader* m_LightShader;
	Text* m_Text;
	BumpMapShader* m_BumpMapShader;
	RefractionShader* m_RefractionShader;
	WaterShader* m_WaterShader;
	OceanShader* m_OceanShader;
	Terrain* m_Terrain;
	TerrainShader* m_TerrainShader;
	ParticleSystem* m_ParticleSystem;
	ParticleShader* m_ParticleShader;
	FireClass* m_Fire;
	FireShader* m_FireShader;
	FogShader* m_FogShader;
	

	//used to keeep track of water position + height
	float m_waterHeight, m_waterTranslation;
};

#endif