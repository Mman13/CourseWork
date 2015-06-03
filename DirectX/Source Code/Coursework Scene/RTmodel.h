////////////////////////////////////////////////////////////////////////////////
// Filename: RTmodel.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RTMODEL_H_
#define _RTMODEL_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"
#include "LightClass.h"
#include "TextureArray.h"

/////////////////////////
// Class name: RTmodel //
/////////////////////////
class RTModel
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
	float x,y,z;
	float tu, tv;
	float nx,ny,nz;
	};

public:
	RTModel();
	RTModel(const RTModel&);
	~RTModel();

	bool Initialize(ID3D11Device*device, char* modelFilename, WCHAR* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView** GetTextureArray();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char* filename);
	void ReleaseModel();
	

	private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	TextureArray* m_TextureArray;
	ModelType* m_model;
};

#endif