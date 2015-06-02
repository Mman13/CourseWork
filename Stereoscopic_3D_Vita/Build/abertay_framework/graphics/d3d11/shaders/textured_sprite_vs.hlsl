cbuffer MatrixBuffer
{
	matrix projection;
	matrix sprite_data;
};

struct VertexInput
{
    float4 position : POSITION;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
    float2 uv : TEXCOORD;
};
void VS( in VertexInput input,
         out PixelInput output )
{
    //float2 trans_vert = float2(sprite_data[2][0], sprite_data[2][1]) + mul(float2x2(sprite_data[0][0], sprite_data[0][1], sprite_data[1][0], sprite_data[1][1]), input.position.xy);

	float2 trans_vert =
		float2(sprite_data[2][0], sprite_data[2][1]) +
		mul(float2x2(sprite_data[0][0], sprite_data[1][0], sprite_data[0][1], sprite_data[1][1]), input.position.xy); // column major
		
	output.uv = (input.position.xy + float2(0.5, 0.5)) * float2(sprite_data[1][2], sprite_data[1][3]) + float2(sprite_data[0][2], sprite_data[0][3]);
    output.position = mul(float4(trans_vert.x, trans_vert.y, sprite_data[2][2], 1), projection);
    output.colour = float4( sprite_data[3][0], sprite_data[3][1], sprite_data[3][2], sprite_data[3][3] );
}