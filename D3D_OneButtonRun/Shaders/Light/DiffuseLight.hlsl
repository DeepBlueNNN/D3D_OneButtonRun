#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv  : UV;
    float3 diffuse : DIFFUSE;
};

// shader program
//시멘틱(Sementic) : 반환값이나 매개변수가 어떻게 사용할지 나타내는 것
//SV(SystemValue)
PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
    
    // 법선벡터를 월드행렬로 변환
    output.diffuse = normalize(mul(input.normal, (float3x3) world));
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float3 normal = normalize(input.diffuse);
    float3 light  = normalize(lightDirection);
    float  diffuseIntensity = saturate(dot(normal, -light));
    
    return diffuseMap.Sample(samp, input.uv) * diffuseIntensity;
}
