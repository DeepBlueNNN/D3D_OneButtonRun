#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos     : SV_POSITION;
    float2 uv      : UV;
    float3 normal  : NORMAL;
    float3 viewDir : VIEWDIR;  // CAMERA����
};

// shader program
//�ø�ƽ(Sementic) : ��ȯ���̳� �Ű������� ��� ������� ��Ÿ���� ��
//SV(SystemValue)
PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    
    // ī�޶�� ����   ��ġ������ ����
    output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
    output.viewDir = normalize(invView._31_32_33);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
    
    // Normal������ ���
    output.normal =  mul(input.normal,(float3x3) world);
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(samp, input.uv); // original pixel
    float3 normal = normalize(input.normal);
    float3 light = normalize(lightDirection);
    
    float diffuseIntensity = saturate(dot(normal, -light)); // 0~1���� ��
    
    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        // �����̴� :  Phong Shading
        // ���ݻ�
        float3 reflection = normalize(reflect(light, normal));
        specular = saturate(dot(-reflection, input.viewDir));
        
        float4 speculaIntensity = specularMap.Sample(samp, input.uv);
        specular = pow(specular, shininess) * speculaIntensity;
    }
    
    // ���ݻ� �� : ����
    float4 diffuse = albedo * diffuseIntensity;
    float4 ambient = albedo * 0.2f;
    
    return diffuse + specular + ambient;
}
