#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 originPos : POSITION;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos.xyz = mul(input.pos.xyz, (float3x3) view);
    output.pos.w = 1.0f;
    output.pos = mul(output.pos, projection);

    output.originPos = input.pos.xyz;
	
    return output;
}


Texture2D skyMap : register(t10);

float2 SphereToUV(float3 dir)
{
    float2 uv;
    uv.x = atan2(dir.x, dir.z) / (2.0 * 3.14159265f) + 0.5f;
    uv.y = 0.5f - asin(dir.y) / 3.14159265f; 
    return uv;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float3 dir = normalize(input.originPos);
    float2 uv = SphereToUV(dir);
    return skyMap.Sample(samp, uv);
}
