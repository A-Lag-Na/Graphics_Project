cbuffer dirLight : register (b0)
{
    float4 _dlcol;
    float4 _dldir;
}
cbuffer pointLight : register(b1)
{
    float4 _pointcol;
    float4 _pointpos;
}
cbuffer ambLight : register(b2)
{
    float4 _alcol;
    float4 _aldir;
}
//cbuffer spotLight : register(b3)
//{
//    float4 _slcol;
//    float4 _sldir;
//    float4 _conedir;
//    float4 _coneratio;
//}

Texture2D baseTexture : register(t0);
SamplerState linfilter : register(s0);

float4 unlight = float4(1.f, 1.f, 1.f, 0.f);
float3 zero = float3(0.f, 0.f, 0.f);

struct VS_OUT
{
    float3 localpos : LOCAL_POSITION;
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

//Functional directional light only implementation
//float4 lightColor = dlcol;
//lightColor = saturate(lightColor);
//float3 ldirection = -normalize(dldir);
//float3 wnorm = normalize(input.norm);
//float4 outColor = saturate((dot(ldirection, wnorm))) * dlcol * baseColor;
//return outColor;

//It's a surprise tool we'll need later
//ATTENUATION = 1.0 – CLAMP( ( INNERCONERATIO - SURFACERATIO ) / ( INNERCONERATIO – OUTERCONERATIO ) ) 

float4 calculateDirLight(float4 dlColor, float4 dlDir, float3 surfaceNormal)
{
     float4 lightColor = dlColor;
     float3 ldirection = -normalize(dlDir);
     float3 wnorm = normalize(surfaceNormal);
     float4 outColor = saturate((dot(ldirection, wnorm))) * dlColor;
     //return outColor;
     return outColor;
}

//Point light formula implementation
//lightDir = normalize(pointpos - input.pos);
//lightRatio = saturate(dot(lightDir, input.norm));
//outColor.xyz = lightRatio * pointcol * baseColor;
//outColor.a = baseColor.a;
//return outColor;

//It's a surprise tool we'll use later
//ATTENUATION = 1.0 – CLAMP( MAGNITUDE(
//LIGHTPOS– SURFACEPOS) / LIGHTRADIUS ) 

float4 calculatePointLight(float4 pointColor, float4 pointPos, float3 surfaceNormal, float4 surfacePosition)
{
    float3 lightDir = normalize(pointPos - surfacePosition);
    float4 lightRatio = saturate(dot(lightDir, surfaceNormal));
    return saturate(lightRatio * pointColor);
}

//Spotlight formula from slides
//LIGHTDIR = NORMALIZE(LIGHTPOS– SURFACEPOS) )
//SURFACERATIO = CLAMP( DOT( -LIGHTDIR, CONEDIR ) )
//SPOTFACTOR = ( SURFACERATIO > CONERATIO ) ? 1 : 0
//LIGHTRATIO = CLAMP( DOT( LIGHTDIR, SURFACENORMAL ) )
//RESULT = SPOTFACTOR * LIGHTRATIO * LIGHTCOLOR * SURFACECOLOR

//float4 calculateSpotLight(float3 surfaceNormal, float4 surfacePosition)
//{
//    if(spotpos && spotcol && conedir && coneratio)
//    {
//        float3 lightDir = normalize(spotpos - surfacePosition);
//        float4 surfaceRatio = saturate(-lightDir, coneDir);
//        float4 spotFactor = (surfaceRatio > coneRatio) ? 1 : 0;
//        float4 lightRatio = saturate(dot(lightDir, surfaceNormal));
//        float4 outColor = spotFactor * lightRatio * spotcol;
//        return outColor;
//    }
//    else
//    {
//        return unlight;
//    }
//}

float4 calculateAmbLight(float4 alColor)
{
    return alColor;
}

float4 main(VS_OUT input) : SV_TARGET
{
    float4 baseColor;
    float4 outColor;
    float4 dlCol = _dlcol;
    float4 dlDir = _dldir;
    float4 pointColor = _pointcol;
    float4 pointPos = _pointpos;
    float4 alColor = _alcol;
    //TODO: Change these to passed in spotlight values
    //float4 dlCol = _dlcol;
    //float4 dlCol = _dlcol;
    //float4 dlCol = _dlcol;
    //float4 dlCol = _dlcol;
    
    //Get the base color from the texture file
	baseColor = baseTexture.Sample(linfilter, input.tex);
    
    //I think the commented out code below is non-functional right now
    //if(baseColor.x == zero.x && baseColor.y == zero.y && baseColor.z == zero.z)
    //{
    //    baseColor = float4(input.localpos, 1.f);
    //}
    
    float4 dlOut = calculateDirLight(dlCol, dlDir, input.norm);
    float4 pointOut = calculatePointLight(pointColor, pointPos, input.norm, input.pos);
    float4 ambOut = calculateAmbLight(alColor);
    float4 outputs[3] = { dlOut, pointOut, ambOut };
    
    //This loop should "correct" black lights to have no impact on the overall color of the output.
    for (int i = 0; i < 3; i++)
    {
        if (outputs[i].x == zero.x && outputs[i].y == zero.y && outputs[i].z == zero.z)
        {
            outputs[i] = unlight;
        }
    }

    outColor.xyz = saturate(outputs[0].xyz + outputs[1].xyz + outputs[2].xyz) * baseColor.xyz;
    outColor.a = baseColor.a;
    return outColor;
    
    //If ambient lighting, return this instead
    //return saturate(outColor + ambOutput);
    
    //For debugging, use .pos or .norm here
    //return float4(input.localpos, 1.0f);
}