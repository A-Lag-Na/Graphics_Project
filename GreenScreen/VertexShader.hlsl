//cbuffer WVP : register (b0)
//{
//	matrix w, v, p;
//}

struct VS_IN
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};
struct VS_OUT
{
    float3 localpos : LOCAL_POSITION;
	float4 pos : SV_POSITION;
    float3 norm : NORMAL;
	float2 tex : TEXCOORD;
};

//Commented out code is for lighting and/or texturing

VS_OUT main(VS_IN input)
{
	VS_OUT output;
    output.pos = float4(input.pos, 1.0f);
    output.norm = input.norm;
    output.tex = input.tex;
    output.localpos = 0;
 //   output.localpos = input.pos;
	//output.pos = float4(input.pos.xyz, 1.0f);
	//output.pos = mul(output.pos, w);
	//output.pos = mul(output.pos, v);
	//output.pos = mul(output.pos, p);
	//output.tex = input.tex;
	//output.norm = float4(input.norm.xyz, 0.0f);
 //   output.norm = mul(output.norm, (float3x3) w).xyz;
	return  output;
}