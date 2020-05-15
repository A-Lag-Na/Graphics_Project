cbuffer dirLight : register (b0)
{
    float4 dlcol;
    float4 dldir;
}
cbuffer pointLight : register(b1)
{
    float4 pointcol;
    //Point lights are inherently non-directional, so pointdir will be unused, however, to make sure the Light data is passed correctly, I'm declaring it anyways.
    float4 pointpos;
}
cbuffer ambLight : register(b2)
{
    float4 alcol;
    float4 aldir;
}

Texture2D baseTexture : register(t0);
SamplerState linfilter : register(s0);

struct VS_OUT
{
    float3 localpos : LOCAL_POSITION;
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

//Point light formula implementation
//lightDir = normalize(pointpos - input.pos);
//lightRatio = saturate(dot(lightDir, input.norm));
//outColor.xyz = lightRatio * pointcol * baseColor;
//outColor.a = baseColor.a;
//return outColor;
float4 calculatePointLight(float3 surfaceNormal, float4 surfacePosition, float4 baseColor)
{
    float3 lightDir = normalize(pointpos - surfacePosition);
    float4 lightRatio = saturate(dot(lightDir, surfaceNormal));
    return lightRatio * pointcol * baseColor;
}


//Functional directional light only implementation
//float4 lightColor = dlcol;
//lightColor = saturate(lightColor);
//float3 ldirection = -normalize(dldir);
//float3 wnorm = normalize(input.norm);
//float4 outColor = saturate((dot(ldirection, wnorm))) * dlcol * baseColor;
//return outColor;
float4 calculateDirLight(float3 surfaceNormal, float4 baseColor)
{
    float4 lightColor = dlcol;
    float3 ldirection = -normalize(dldir);
    float3 wnorm = normalize(surfaceNormal);
    float4 outColor = saturate((dot(ldirection, wnorm))) * dlcol * baseColor;
    return outColor;
}

float4 main(VS_OUT input) : SV_TARGET
{
    //Forward declarations because I gotta for outColor and so I might as well clean up and forward declare all variables here. 
    float4 baseColor, lightRatio, outColor, ambOutput, lightColor;
    float3 lightDir;
    //Get the base color from the texture file
	baseColor = baseTexture.Sample(linfilter, input.tex);
    
    outColor.xyz = saturate(calculateDirLight(input.norm, baseColor).xyz + calculatePointLight(input.norm, input.pos, baseColor).xyz) * baseColor.xyz;
    outColor.a = baseColor.a;
    return outColor;
    
    //Ambient lighting code
    //ambOutput = alcol * baseColor;
    //lightColor = alcol + dlcol;
    
    //If ambient lighting, return this instead
    //return saturate(outColor + ambOutput);
    
    //For debugging, use .pos or .norm here
    //return float4(input.localpos, 1.0f);
}


