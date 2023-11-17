#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"


struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv  : UV;
	float3 normal  : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDir : VIEWDIR;    // CAMERA����
	float3 wolrdPos : POSITION;
};

// shader program
//�ø�ƽ(Sementic) : ��ȯ���̳� �Ű������� ��� ������� ��Ÿ���� ��
//SV(SystemValue)
PixelInput VS(VertexUVNormalTangent input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
    
    // ī�޶�� ����   ��ġ������ ����
//	output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
	output.viewDir = normalize(invView._31_32_33);
	output.wolrdPos = output.pos.xyz;
    
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.uv = input.uv;
    
    // Normal������ ���
	output.normal  = mul(input.normal,  (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv); // original pixel
	
	float3 T = normalize(input.tangent);
	float3 B = normalize(input.binormal);
	float3 N = normalize(input.normal);
	
	float3 normal = N;
	float3 light = normalize(lightDirection);
    
	if(1)  // ���� ����
	{
		// 0~1 ==> -1~1
		float3 normalMapColor = normalMap.Sample(samp, input.uv).rgb;
		normal = normalMapColor * 2.0f - 1.0f;
		float3x3 TBN = float3x3(T, B, N);
		normal = normalize(mul(normal, TBN)); //�鿡 ���������ͷ�
		
	}
	
	
	
	float diffuseIntensity = saturate(dot(normal, -light)); // 0~1���� ��
    
	float4 specular = 0;
	if (diffuseIntensity > 0)
	{
        // �����̴� :  Phong Shading
        // ���ݻ�
		float3 reflection = normalize(reflect(light, normal));
		specular = saturate(dot(-reflection, input.viewDir));
        
		float4 speculaIntensity = speculaMap.Sample(samp, input.uv);
		specular = pow(specular, shininess) * speculaIntensity;
	}
    
    // ���ݻ� �� : ����
	float4 diffuse = albedo * diffuseIntensity;
	float4 ambient = albedo * 0.2f;
	return diffuse + specular + ambient;
}