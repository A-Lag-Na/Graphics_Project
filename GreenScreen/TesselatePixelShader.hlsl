cbuffer time : register(b5)
{
    float4 deltaTime;
}

Texture2D baseTexture : register(t0);
SamplerState linfilter : register(s0);

struct VS_OUT
{
    float3 localpos : LOCAL_POSITION;
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
    float3 worldpos : WORLD_POSITION;
    float4 camerapos : CAMERA_POSITION;
};

float4 main(VS_OUT input) : SV_TARGET
{
    //Waves
    //float frequency, waveHeight = 5.0f;
    input.tex += (deltaTime.x / 1000.f) * float2(0.1f, 0.1f);
    float4 outColor = baseTexture.Sample(linfilter, input.tex);
    return outColor;
}