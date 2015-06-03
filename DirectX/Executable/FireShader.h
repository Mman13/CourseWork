////////////////////////////////////////////////////////////////////////////////
// Filename: FireShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FIRESHADER_H_
#define _FIRESHADER_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: FireShader
////////////////////////////////////////////////////////////////////////////////
class FireShader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct NoiseBufferType
	{
		float frameTime;
		D3DXVECTOR3 scrollSpeeds;
		D3DXVECTOR3 scales;
		float padding;
	};

	struct DistortionBufferType
	{
		D3DXVECTOR2 distortion1;
		D3DXVECTOR2 distortion2;
		D3DXVECTOR2 distortion3;
		float distortionScale;
		float distortionBias;
	};

public:
	FireShader();
	FireShader(const FireShader&);
	~FireShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, 
		    ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, float, float);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, 
				 ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, 
				 D3DXVECTOR2, D3DXVECTOR2, float, float);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	// We have a new buffer for the noise constant buffer in the vertex shader.
	ID3D11Buffer* m_noiseBuffer;
	ID3D11SamplerState* m_sampleState;

	// There is a new sampler state which will use Clamp instead of Wrap for the fire effect.
	ID3D11SamplerState* m_sampleState2;

	// Also there is a new distortion buffer for the distortion constant buffer in the pixel shader.
	ID3D11Buffer* m_distortionBuffer;
};

#endif