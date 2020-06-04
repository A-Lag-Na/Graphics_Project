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
    float3 toCamera = normalize(input.camerapos.xyz - input.worldpos);
    float3 vReflect = reflect(-toCamera, input.norm);
    float4 vReflectColor = diffuse.Sample(samp, input.norm);
    return vReflectColor;
}