#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexInput
{
    float4 pos : POSITION;
    matrix transform : INSTANCE_TRANSFORM; 
};

struct PixelInput
{
    float4 pos : SV_POSITION;
};

PixelInput VS(VertexInput input)// : SV_POSITION//SV->RS
{
    PixelInput output;
   
    float4 worldPos = mul(input.pos, input.transform);
    
    output.pos = mul(worldPos, view);
    output.pos = mul(output.pos, projection);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET //->SV->OM
{
    return mDiffuse;
}