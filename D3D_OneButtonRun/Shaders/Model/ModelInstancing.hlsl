#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexInstancing input)
{
    LightPixelInput output;
    
    output.pos = mul(input.pos, input.transform);
   
    output.viewPos = invView._41_42_43; // Camera : X, Y, Z ÁÂÇ¥
    output.worldPos = output.pos.xyz; // Object : World ÁÂÇ¥
   
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
   
    output.normal = mul(input.normal, (float3x3) input.transform);
    output.tangent = mul(input.tangent, (float3x3) input.transform);
    output.binormal = cross(output.normal, output.tangent);
   
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    Material material = GetMaterial(input);
   
    float4 color = CalcDirectional(material, lights[0]);
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcEmissive(material);
   
    return color + ambient + emissive;
}