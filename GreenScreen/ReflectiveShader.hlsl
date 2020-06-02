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

TextureCube diffuse : register(t0);
SamplerState samp : register(s0);


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
    float3 vReflect = reflect(input.camerapos.xyz, input.norm);
    float4 vReflectColor = diffuse.Sample(samp, vReflect);
    return vReflectColor;
}

//Reflective material half-remembered snippet
//float3 vReflect     = reflect(-lightDir, surfaceNormal); (might be +lightDir)
//float3 vReflectColor = diffuse.Sample(samp, input.tex);
//Probably best in its own pixelShader, can add this to output for lights interaction