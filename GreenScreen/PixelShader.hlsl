cbuffer dirLight : register (b0)
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
    float3 worldpos : WORLD_POSITION;
    float4 camerapos : CAMERA_POSITION;
};

//Specular code: "I fear no man, but that thing... it scares me"
//VIEWDIR = NORMALIZE(CAMWORLDPOS– SURFACEPOS) 
//HALFVECTOR = NORMALIZE((-LIGHTDIR ) + VIEWDIR) 
//INTENSITY = MAX( CLAMP( DOT( NORMAL, NORMALIZE(HALFVECTOR) ))SPECULARPOWER , 0 )
//RESULT = LIGHTCOLOR * SPECULARINTENSITY * INTENSITY 

//Functional directional light only implementation
//float4 lightColor = dlcol;
//lightColor = saturate(lightColor);
//float3 ldirection = -normalize(dldir);
//float3 wnorm = normalize(input.norm);
//float4 outColor = saturate((dot(ldirection, wnorm))) * dlcol * baseColor;
//return outColor;

float3 calculateDirLight(float4 dlColor, float4 dlDir, float3 surfaceNormal, float3 surfacePosition, float3 cameraPos, float specularPower, float specularIntensity)
{
     float4 lightColor = dlColor;
     float3 ldirection = -normalize(dlDir);
     float3 wnorm = normalize(surfaceNormal);
     float4 outColor = saturate((dot(ldirection, wnorm))) * dlColor;
     
    
        //Specular code: "I fear no man, but that thing... it scares me"
//VIEWDIR = NORMALIZE(CAMWORLDPOS– SURFACEPOS) 
//HALFVECTOR = NORMALIZE((-LIGHTDIR ) + VIEWDIR) 
//INTENSITY = MAX( CLAMP( DOT( NORMAL, NORMALIZE(HALFVECTOR) ))SPECULARPOWER , 0 )
//RESULT = LIGHTCOLOR * SPECULARINTENSITY * INTENSITY 
    float3 viewDir = normalize(cameraPos - surfacePosition);
    float3 halfVector = normalize((-ldirection) + viewDir);
    float intensity = max(pow(saturate(dot(surfaceNormal, normalize(halfVector))), specularPower), 0);
    float4 specular = dlColor * specularIntensity * intensity;
    return outColor + specular;
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

float3 calculatePointLight(float4 pointColor, float4 pointPos, float4 pointRad, float3 surfaceNormal, float3 surfacePosition, float3 cameraPos, float specularPower, float specularIntensity)
{
    float3 lightDir = normalize(pointPos.xyz - surfacePosition);
    float lightRatio = saturate(dot(lightDir, surfaceNormal));
    float4 outColor = saturate(lightRatio * pointColor);
    float temp = length(pointPos.xyz - surfacePosition) / pointRad.x; //saturate
    float attenuation = 1.0f - temp;
    outColor *= (attenuation * attenuation);
   
    
    // alternate method (thanks Clark and Dan)
    //As specular power (specular hardness) increases, highlights become narrower, specular intensity is a brightness multiplier.
    if (outColor.x == zero.x && outColor.y == zero.y && outColor.z == zero.z)
    {
        return outColor;
    }
    else
    {
        float3 vToCamera = normalize(cameraPos - surfacePosition);
        float3 vReflect = reflect(-lightDir, surfaceNormal);
        float fSpecDot = saturate(dot(vToCamera, vReflect));
        fSpecDot = pow(fSpecDot, specularPower);
        float4 specular = float4(1.0f, 1.0f, 1.0f, 1.0f) * specularIntensity * fSpecDot;
        return outColor + specular;
    }    
}

//Spotlight formula from slides
//LIGHTDIR = NORMALIZE(LIGHTPOS– SURFACEPOS) )
//SURFACERATIO = CLAMP( DOT( -LIGHTDIR, CONEDIR ) )
//SPOTFACTOR = ( SURFACERATIO > CONERATIO ) ? 1 : 0
//LIGHTRATIO = CLAMP( DOT( LIGHTDIR, SURFACENORMAL ) )
//RESULT = SPOTFACTOR * LIGHTRATIO * LIGHTCOLOR * SURFACECOLOR

//It's a surprise tool we'll need later
//ATTENUATION = 1.0 – CLAMP( ( INNERCONERATIO - SURFACERATIO ) / ( INNERCONERATIO – OUTERCONERATIO ) ) 

float3 calculateSpotLight(float4 spotColor, float4 spotPos, float4 coneDir, float4 coneRatio, float3 surfaceNormal, float3 surfacePosition, float3 cameraPos, float specularPower, float specularIntensity)
{
    //points towards the light
    float3 lightDir = normalize(spotPos.xyz - surfacePosition);
    float surfaceRatio = saturate(dot(-lightDir, normalize(coneDir.xyz)));
    //replace this with attenuation
    //float spotFactor = (surfaceRatio > coneRatio.x) ? 1 : 0;
    float lightRatio = saturate(dot(lightDir, normalize(surfaceNormal)));
    //mult spot factor
    float4 outColor = lightRatio * spotColor;
    //coneRatio. y is inner cone, coneRatio.x is outer. 2.0 is hardcoded radius for spotlight
    float attenuation1 = 1.0f - length(spotPos.xyz - surfacePosition) / 2.0f;
    float attenuation2 = 1.0f - saturate((coneRatio.y - surfaceRatio) / (coneRatio.y - coneRatio.x));
    outColor *= (attenuation1 * attenuation2);
    
    float3 viewDir = normalize(cameraPos - surfacePosition);
    float3 halfVector = normalize((-lightDir) + viewDir);
    float intensity = max(pow(saturate(dot(surfaceNormal, normalize(halfVector))), specularPower), 0);
    float3 specular = spotColor * specularIntensity * intensity;
    return outColor.xyz + specular;
}

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
    float4 pointRad = _pointrad;
    float4 alColor = _alcol;
    float4 spotColor = _slcol;
    float4 spotPos = _sldir;
    float4 coneDir = _conedir;
    float4 coneRatio = _coneratio;
    float4 cameraPos = input.camerapos;
    float specularPower = 32.0f;
    float specularIntensity = 2.0f;
    
    //Get the base color from the texture file
	baseColor = baseTexture.Sample(linfilter, input.tex);
    
    float3 dlOut = calculateDirLight(dlCol, dlDir, input.norm, input.worldpos, cameraPos.xyz, specularPower, specularIntensity);
    float3 pointOut = calculatePointLight(pointColor, pointPos, pointRad, input.norm, input.worldpos, cameraPos.xyz, specularPower, specularIntensity);
    float3 ambOut = calculateAmbLight(alColor);
    float3 spotOut = calculateSpotLight(spotColor, spotPos, coneDir, coneRatio, input.norm, input.worldpos, cameraPos.xyz, specularPower, specularIntensity);
    float3 outputs[4] = { dlOut, pointOut, ambOut,  spotOut};
    
    //This loop should "correct" black lights to have no impact on the overall color of the output.
    for (int i = 0; i < 3; i++)
    {
        if (outputs[i].x == zero.x && outputs[i].y == zero.y && outputs[i].z == zero.z)
        {
            outputs[i] = unlight;
        }
    }

    outColor.xyz = saturate(outputs[0].xyz + outputs[1].xyz + outputs[2].xyz + outputs[3].xyz) * baseColor.xyz;
    outColor.a = baseColor.a;
    return outColor;
}