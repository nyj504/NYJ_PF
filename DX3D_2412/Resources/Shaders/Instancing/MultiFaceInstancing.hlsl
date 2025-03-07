#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexInput
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
	
    matrix transform : INSTANCE_TRANSFORM;
    float2 curFrame : INSTANCE_CURFRAME;
    float2 maxFrame : INSTANCE_MAXFRAME;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : POSITION0;
    float3 viewPos : POSITION1;
    
    float2 curFrame : INSTANCE_CURFRAME;
    float2 maxFrame : INSTANCE_MAXFRAME;
    
};

Material GetMaterial(PixelInput input)
{
    Material material;
   
    float2 uv = input.curFrame + input.uv;
    
    material.baseColor = diffuseMap.Sample(samp, uv);
    material.normal = GetNormal(input.tangent, input.binormal, input.normal, uv);
    material.specularIntensity = specularMap.Sample(samp, uv);
    material.worldPos = input.worldPos;
    material.viewDir = normalize(input.worldPos - input.viewPos);
	
    return material;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    float4 worldPos = mul(input.pos, input.transform);
    
    output.pos = mul(worldPos, view);
    output.pos = mul(output.pos, projection);
    
    output.worldPos = worldPos.xyz;
    
    float3x3 normalMatrix = (float3x3) input.transform;

    output.normal = mul(input.normal, normalMatrix);
    output.tangent = mul(input.tangent, normalMatrix);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    output.curFrame = input.curFrame;
    output.maxFrame = input.maxFrame;
    
    output.viewPos = invView._41_42_43;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    Material material = GetMaterial(input);
  
    float4 ambient = CalcAmbient(material);
    float4 result = 0;
    
    [unroll(MAX_LIGHT)]
    for (int i = 0; i < lightCount; i++)
    {
        if (!lights[i].isActive)
            continue;

        if (lights[i].type == 0)
            result += CalcDirectional(material, lights[i]);
        else if (lights[i].type == 1)
            result += CalcPoint(material, lights[i]);
        else if (lights[i].type == 2)
            result += CalcSpot(material, lights[i]);
    }
    
    return (result + ambient + mEmissive) + material.baseColor;
}