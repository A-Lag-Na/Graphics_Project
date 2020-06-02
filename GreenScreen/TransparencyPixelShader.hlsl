Texture2D diffuse : register(t0);
SamplerState samp : register(s0);

cbuffer TransparentBuffer : register(b0)
{
    float4 blendAmount;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
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