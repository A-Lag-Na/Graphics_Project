Texture2D diffuse : register(t0);
SamplerState samp : register(s0);

cbuffer TransparentBuffer : register(b6)
{
    float4 blendAmount;
};

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
    float4 color;

    // Sample the texture pixel at this location.
    color = diffuse.Sample(samp, input.tex);
    
   // Set the alpha value of this pixel to the blending amount to create the alpha blending effect.
    color.a = blendAmount.x;

    return color;
}