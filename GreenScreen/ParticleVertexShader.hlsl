#pragma pack_matrix(row_major)

cbuffer WVP : register(b0)
{
    matrix w, v, p;
}

struct VS_IN
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 color : COLOR;
};
VS_OUT main(VS_IN input)
{
    VS_OUT output;
    
   input.pos = float4(input.pos, 1.0f);

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.pos, w);
    output.position = mul(output.position, v);
    output.position = mul(output.position, p);
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	// Store the particle color for the pixel shader. 
    output.color = input.norm;

    return output;
}