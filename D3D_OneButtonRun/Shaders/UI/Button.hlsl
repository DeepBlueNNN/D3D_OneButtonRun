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
    float hoverAlpha;
    float margin;
}

Texture2D hoverTexture : register(t20);
Texture2D outlineTexture : register(t21);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 srcTex = diffuseMap.Sample(samp, input.uv);
    
    if (margin > 0.0f)
    {
        if (input.uv.x <= margin || input.uv.y <= margin || 1-input.uv.x <= margin || 1- input.uv.y <= margin)
            srcTex = outlineTexture.Sample(samp, input.uv);
    }
    
    if (hoverAlpha > 0.0f)
    {
        srcTex = srcTex * (hoverTexture.Sample(samp, input.uv * hoverAlpha));
    }
    
    return srcTex;
}