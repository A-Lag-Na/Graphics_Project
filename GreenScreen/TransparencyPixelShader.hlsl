Texture2D diffuse : register(t0);
SamplerState samp : register(s0);

cbuffer dirLight : register(b0)
{
    float4 _dlcol;
    float4 _dldir;
}
cbuffer pointLight : register(b1)
{
    float4 _pointcol;
    float4 _pointpos;
    float4 _pointrad;
}
cbuffer ambLight : register(b2)
{
    float4 _alcol;
    float4 _aldir;
}
cbuffer spotLight : register(b3)
{
    float4 _slcol;
    float4 _sldir;
    float4 _conedir;
    float4 _coneratio;
}
cbuffer TransparentBuffer : register(b4)
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