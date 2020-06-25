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
    float4 pos : SV_POSITION;
    float3 tex : TEXCOORD;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    
    // project the verts into WVP
    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, w);
    output.pos = mul(output.pos, v);
    output.pos = mul(output.pos, p);
    
    
    output.tex = input.pos;
    
    return output;
}