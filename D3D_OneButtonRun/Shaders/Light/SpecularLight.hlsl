#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos     : SV_POSITION;
    float2 uv      : UV;
    float3 normal  : NORMAL;
    float3 viewDir : VIEWDIR;  // CAMERA방향
};

// shader program
//시멘틱(Sementic) : 반환값이나 매개변수가 어떻게 사용할지 나타내는 것
//SV(SystemValue)
PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    
    // 카메라와 현재   위치점과의 방향
    output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
    output.viewDir = normalize(invView._31_32_33);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;
    
    // Normal데이터 계산
    output.normal =  mul(input.normal,(float3x3) world);
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(samp, input.uv); // original pixel
    float3 normal = normalize(input.normal);
    float3 light = normalize(lightDirection);
    
    float diffuseIntensity = saturate(dot(normal, -light)); // 0~1사의 값
    
    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        // 퐁쉐이더 :  Phong Shading
        // 정반사
        float3 reflection = normalize(reflect(light, normal));
        specular = saturate(dot(-reflection, input.viewDir));
        
        float4 speculaIntensity = specularMap.Sample(samp, input.uv);
        specular = pow(specular, shininess) * speculaIntensity;
    }
    
    // 난반사 값 : 감쇄
    float4 diffuse = albedo * diffuseIntensity;
    float4 ambient = albedo * 0.2f;
    
    return diffuse + specular + ambient;
}
