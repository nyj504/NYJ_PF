#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float3 originPos : POSITION;
};

cbuffer TimeBuffer : register(b10)
{
    float time;
}

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos.xyz = mul(input.pos.xyz, (float3x3) view);
    output.pos.w = 1.0f;
    output.pos = mul(output.pos, projection);

    output.originPos = input.pos.xyz;
	
    return output;
}

Texture2D skyMapStart : register(t10); 
Texture2D skyMapEnd : register(t11);

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
    
    float4 startColor = skyMapStart.Sample(samp, uv); 
    float4 endColor = skyMapEnd.Sample(samp, uv); 
    //시간이 흐르면서 자연스럽게 하늘이 섞임
    return lerp(startColor, endColor, time) * mDiffuse;
}
