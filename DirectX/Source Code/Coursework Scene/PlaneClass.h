////////////////////////////////////////////////////////////////////////////////
// Filename: PlaneClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PLANECLASS_H_
#define _PLANECLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"
#include "LightClass.h"
#include "TextureArray.h"

//////////////////////////////////////////////////////////
// Class name: PlaneClass
//////////////////////////////////////////////////////////
class PlaneClass
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

public:
	PlaneClass();
	PlaneClass(const PlaneClass&);
	~PlaneClass();

	bool Initialize(ID3D11Device*, WCHAR* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int QuadCount;

	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView** GetTextureArray();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	TextureArray* m_TextureArray;
};

#endif
