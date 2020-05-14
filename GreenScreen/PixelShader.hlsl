// Commented out lighting and texturing until those features are implemented.
cbuffer dirLight : register (b1)
{
	float4 dldir;
	float4 dlcol;
}

/*cbuffer ambLight : register(b2)
{
    float4 aldir;
    float4 alcol;
}*/

Texture2D baseTexture : register(t0);
SamplerState linfilter : register(s0);

struct VS_OUT
{
    float3 localpos : LOCAL_POSITION;
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

float4 main(VS_OUT input) : SV_TARGET
{
    //Commented out code is for ambient lighting + directional lighting
	float4 baseColor = baseTexture.Sample(linfilter, input.tex); // get base color
    // float4 ambient = alcol * baseColor;
    //float4 lightColor = alcol + dlcol;
    //lightColor = saturate(lightColor);
    float4 lightColor = dlcol;
	float3 ldirection = -normalize(dldir);
	float3 wnorm = normalize(input.norm);
    float4 outColor = saturate((dot(ldirection, wnorm))) * dlcol * baseColor;
    return outColor;
    //return saturate(outColor + ambient);
    
    //Debugging, use .pos or .norm here
    //return float4(input.localpos, 1.0f);
    //return baseColor;
}