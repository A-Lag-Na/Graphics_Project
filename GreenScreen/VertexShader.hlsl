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
    //float3 camerapos : CAMERA_POSITION;
};
struct VS_OUT
{
    float3 localpos : LOCAL_POSITION;
	float4 pos : SV_POSITION;
    float3 norm : NORMAL;
	float2 tex : TEXCOORD;
    float3 worldpos : WORLD_POSITION;
    //float3 camerapos : CAMERA_POSITION;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;
    output.localpos = input.pos.xyz;
    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, w);
    output.worldpos = output.pos;
    output.pos = mul(output.pos, v);
    output.pos = mul(output.pos, p);
    output.tex = input.tex;
    output.norm = mul(float3(input.norm), (float3x3) w).xyz;
	return  output;
}