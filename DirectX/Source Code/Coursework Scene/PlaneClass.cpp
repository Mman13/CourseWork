////////////////////////////////////////////////////////////////////////////////
// Filename: PlaneClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PlaneClass.h"

PlaneClass::PlaneClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_TextureArray = 0;
	QuadCount = 1000;
}

PlaneClass::PlaneClass(const PlaneClass& other)
{
}

PlaneClass::~PlaneClass()
{
}

bool PlaneClass::Initialize(ID3D11Device* device, WCHAR* textureFilename)
{
	bool result;


	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

void PlaneClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Release the vertex and index buffers.
	ShutdownBuffers();

	return;
}

void PlaneClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int PlaneClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* PlaneClass::GetTexture()
{
	return m_Texture->GetTexture();
}

ID3D11ShaderResourceView** PlaneClass::GetTextureArray()
{
	return m_TextureArray->GetTextureArray();
}

bool PlaneClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	// Set the number of vertices in the vertex array.
	m_vertexCount = 6 * (QuadCount*QuadCount);

	// Set the number of indices in the index array.
	m_indexCount = 6 * (QuadCount*QuadCount);

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	for(int j = 0; j < QuadCount; j+=6)
	{

		for(int i = 0; i < QuadCount; i+=6)
		{

		// Load the vertex array with data.
		vertices[i+((j*QuadCount)*6)].position = D3DXVECTOR3(i, 0.0f, j - 6.0f);  // Bottom left.
		vertices[i+((j*QuadCount)*6)].texture = D3DXVECTOR2(1.0f, 0.0f);
		vertices[i+((j*QuadCount)*6)].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		vertices[i+1+((j*QuadCount)*6)].position = D3DXVECTOR3(i, 0.0f, j);  // Top left.
		vertices[i+1+((j*QuadCount)*6)].texture = D3DXVECTOR2(0.0f, 0.0f);
		vertices[i+1+((j*QuadCount)*6)].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		vertices[i+2+((j*QuadCount)*6)].position = D3DXVECTOR3(i + 6.0f, 0.0f, j);  // top right.
		vertices[i+2+((j*QuadCount)*6)].texture = D3DXVECTOR2(0.0f, 1.0f);
		vertices[i+2+((j*QuadCount)*6)].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		vertices[i+3+((j*QuadCount)*6)].position = D3DXVECTOR3(i + 6.0f, 0.0f, j);  // top right.
		vertices[i+3+((j*QuadCount)*6)].texture = D3DXVECTOR2(0.0f, 1.0f);
		vertices[i+3+((j*QuadCount)*6)].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		vertices[i+4+((j*QuadCount)*6)].position = D3DXVECTOR3(i + 6.0f, 0.0f, j - 6.0f);  // bottom right.
		vertices[i+4+((j*QuadCount)*6)].texture = D3DXVECTOR2(1.0f, 1.0f);
		vertices[i+4+((j*QuadCount)*6)].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		vertices[i+5+((j*QuadCount)*6)].position = D3DXVECTOR3(i, 0.0f, j - 6.0f);  // Bottom left.
		vertices[i+5+((j*QuadCount)*6)].texture = D3DXVECTOR2(1.0f, 0.0f);
		vertices[i+5+((j*QuadCount)*6)].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


	
		// Load the index array with data.
		indices[i+((j*QuadCount)*6)] = i+((j*QuadCount)*6);  // Bottom left.
		indices[i+((j*QuadCount)*6)+1] = i+((j*QuadCount)*6)+1;  // Top left.
		indices[i+((j*QuadCount)*6)+2] = i+((j*QuadCount)*6)+2;  // top right.	

		indices[i+((j*QuadCount)*6)+3] = i+((j*QuadCount)*6)+3; //Top right.
		indices[i+((j*QuadCount)*6)+4] = i+((j*QuadCount)*6)+4; //bottom right.
		indices[i+((j*QuadCount)*6)+5] = i+((j*QuadCount)*6)+5; //bottom left

		}
		
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void PlaneClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void PlaneClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool PlaneClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_TextureArray = new TextureArray;
	if(!m_TextureArray)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_TextureArray->Initialize(device, filename, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

void PlaneClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}