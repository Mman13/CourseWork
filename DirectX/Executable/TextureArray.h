////////////////////////////////////////////////////////////////////////////////
// Filename: TextureArray.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREARRAY_H_
#define _TEXTUREARRAY_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>

//////////////////////////////
// Class name: TextureArray //
//////////////////////////////
class TextureArray
{
public:
	TextureArray();
	TextureArray(const TextureArray&);
	~TextureArray();

	bool Initialize(ID3D11Device*, WCHAR*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:

	ID3D11ShaderResourceView* m_textures[2];
};

#endif