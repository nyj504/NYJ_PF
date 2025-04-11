#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

cbuffer TimeBuffer : register(b10)
{
    float time;
}

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.uv = input.uv;
	
    return output;
}

Texture2D lobbyStart : register(t10);
Texture2D lobbyEnd : register(t11);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 startColor = lobbyStart.Sample(samp, input.uv);
    float4 endColor = lobbyEnd.Sample(samp, input.uv);
    
    return lerp(startColor, endColor, time) * mDiffuse;
}
