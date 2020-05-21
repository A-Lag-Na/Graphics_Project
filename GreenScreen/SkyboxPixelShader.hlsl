
TextureCube diffuse : register(t0);
SamplerState samp : register(s0);


struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 tex : TEXCOORD;
};

float4 main(VS_OUT input) : SV_TARGET
{
    return diffuse.Sample(samp, input.tex);
}