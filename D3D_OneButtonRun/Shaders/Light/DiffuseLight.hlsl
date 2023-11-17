#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv  : UV;
    float3 diffuse : DIFFUSE;
};

// shader program
//�ø�ƽ(Sementic) : ��ȯ���̳� �Ű������� ��� ������� ��Ÿ���� ��
//SV(SystemValue)
PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
    
    // �������͸� ������ķ� ��ȯ
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
