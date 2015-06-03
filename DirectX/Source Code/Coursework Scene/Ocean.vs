////////////////////////////////////////////////////////////////////////////////
// Filename: Ocean.vs
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBuffer
{
	float time;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType OceanVertexShader(VertexInputType input)
{
    PixelInputType output;
    float waveHeight = 1.5f;

    input.position.w = 1.0f;

    //offset position based on sine wave
    input.position.y += waveHeight*sin(input.position.x + time);

    //modify normals
    input.normal.x = 1 - cos(input.position.x + time);
    input.normal.y = abs(cos(input.position.x + time));

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    output.normal = normalize(output.normal);

    return output;
}
