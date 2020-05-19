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
//cbuffer spotLight : register(b3)
//{
//    float4 slcol;
//    float4 sldir;
//    float4 conedir;
//    float4 coneratio;
//}

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

//It's a surprise tool we'll use later
//ATTENUATION = 1.0 – CLAMP( MAGNITUDE(
//LIGHTPOS– SURFACEPOS) / LIGHTRADIUS ) 

float4 calculatePointLight(float3 surfaceNormal, float4 surfacePosition, float4 baseColor)
{
    float3 lightDir = normalize(pointpos - surfacePosition);
    float4 lightRatio = saturate(dot(lightDir, surfaceNormal));
    return saturate(lightRatio * pointcol);
}


//Functional directional light only implementation
//float4 lightColor = dlcol;
//lightColor = saturate(lightColor);
//float3 ldirection = -normalize(dldir);
//float3 wnorm = normalize(input.norm);
//float4 outColor = saturate((dot(ldirection, wnorm))) * dlcol * baseColor;
//return outColor;

//It's a surprise tool we'll need later
//ATTENUATION = 1.0 – CLAMP( ( INNERCONERATIO - SURFACERATIO ) / ( INNERCONERATIO – OUTERCONERATIO ) ) 

float4 calculateDirLight(float3 surfaceNormal, float4 baseColor)
{
    float4 lightColor = dlcol;
    float3 ldirection = -normalize(dldir);
    float3 wnorm = normalize(surfaceNormal);
    float4 outColor = saturate((dot(ldirection, wnorm))) * dlcol;
    //return outColor;
    return baseColor;
}

//Spotlight formula from slides
//LIGHTDIR = NORMALIZE(LIGHTPOS– SURFACEPOS) )
//SURFACERATIO = CLAMP( DOT( -LIGHTDIR, CONEDIR ) )
//SPOTFACTOR = ( SURFACERATIO > CONERATIO ) ? 1 : 0
//LIGHTRATIO = CLAMP( DOT( LIGHTDIR, SURFACENORMAL ) )
//RESULT = SPOTFACTOR * LIGHTRATIO * LIGHTCOLOR * SURFACECOLOR

float4 calculateSpotLight(float3 surfaceNormal, float4 surfacePosition)
{
    float3 lightDir = normalize(spotpos - surfacePosition);
    float4 surfaceRatio = saturate(-lightDir, coneDir);
    float4 spotFactor = (surfaceRatio > coneRatio) ? 1 : 0;
    float4 lightRatio = saturate(dot(lightDir, surfaceNormal));
    float4 outColor = spotFactor * lightRatio * spotcol;
    return outColor;
}

float4 calculateAmbLight(float4 baseColor)
{
    return alcol;
}

float4 main(VS_OUT input) : SV_TARGET
{
    float4 baseColor, outColor;
    //Get the base color from the texture file
	baseColor = baseTexture.Sample(linfilter, input.tex);
    outColor.xyz = saturate(calculateDirLight(input.norm, baseColor).xyz + calculatePointLight(input.norm, input.pos, baseColor).xyz + calculateAmbLight(baseColor).xyz) * baseColor.xyz;
    outColor.a = baseColor.a;
    return outColor;
    
    //If ambient lighting, return this instead
    //return saturate(outColor + ambOutput);
    
    //For debugging, use .pos or .norm here
    //return float4(input.localpos, 1.0f);
}


