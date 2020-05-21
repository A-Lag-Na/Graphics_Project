cbuffer GradientBuffer: register(b1)
{
    float4 apexColor;
    float4 centerColor;
};

struct VS_OUT
{
    float3 localpos : LOCAL_POSITION;
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

float4 main(VS_OUT input) : SV_TARGET
{
    float height;
    float4 outputColor;


    // Determine the position on the sky dome where this pixel is located.
    height = input.localpos.y;

    // The value ranges from -1.0f to +1.0f so change it to only positive values.
    if (height < 0.0)
    {
        height = 0.0f;
    }

    // Determine the gradient color by interpolating between the apex and center based on the height of the pixel in the sky dome.
    outputColor = lerp(centerColor, apexColor, height);

    return outputColor;
}