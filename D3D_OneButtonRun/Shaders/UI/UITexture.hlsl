#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    return output;
}

cbuffer ValueBuffer : register(b10)
{
    float alpha;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 srcTex = diffuseMap.Sample(samp, input.uv);
    
    if (alpha < 1.0f)
    {
        srcTex.a = alpha;
    }
    
    return srcTex;
}