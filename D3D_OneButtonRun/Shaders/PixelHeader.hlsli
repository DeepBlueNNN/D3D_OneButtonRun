#define MAX_LIGHT 10

//Pixel Struct
struct Material
{
    float3 normal;
    float4 diffuseColor;
    float4 specularIntensity;
    float3 viewPos;        // VS에서 추가
    float3 worldPos;       // VS에서 추가
};

struct Light
{
    float4  color;
    float3  direction;
    int     type;
};

struct LightPixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewPos : POSITION0;    // VS에서 추가: Camera
    float3 worldPos : POSITION1;   // VS에서 추가: Object 위치
};

//Pixel ConstBuffer
cbuffer LightBuffer : register(b0)
{
    Light lights[MAX_LIGHT];
	
    uint lightCount;
	
    float3 ambientLight;
    float3 ambientCeil;
}

cbuffer MaterialBuffer : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
	
    float shininess;
    int hasNormalMap;
}

//Texture & Sampler
SamplerState samp     :  register(s0);

// Material
Texture2D diffuseMap  :  register(t0);
Texture2D specularMap :  register(t1);
Texture2D normalMap   :  register(t2);

//Pixel Function
float3 NormalMapping(float3 T, float3 B, float3 N, float2 uv)
{
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
	
	[flatten]
    if (hasNormalMap)
    {
        float3 normal = normalMap.Sample(samp, uv).rgb;
        normal = normal * 2.0f - 1.0f; //0~1 -> -1~1
        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(normal, TBN));
    }
	
    return N;
}


Material GetMaterial(LightPixelInput input)
{
    Material material;
    material.normal = NormalMapping(input.tangent,
		input.binormal, input.normal, input.uv);
		
    material.diffuseColor = diffuseMap.Sample(samp, input.uv);
    material.specularIntensity = specularMap.Sample(samp, input.uv);
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
	
    return material;
}

float4 CalcAmbient(Material material)
{
    float up = material.normal.y * 0.5f + 0.5f;
	
    float4 resultAmbient = float4(ambientLight + up * ambientCeil, 1.0f);
	
    return resultAmbient * material.diffuseColor * mAmbient;
}

float4 CalcEmissive(Material material)
{
    float emissiveIntensity = 0.0f;
	
	[flatten]
    if (mEmissive.a > 0.0f)
    {
        float3 viewDir = normalize(material.viewPos - material.worldPos);

        float t = saturate(dot(material.normal, viewDir));
        emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
    }
	
    return mEmissive * emissiveIntensity;
}

float4 CalcDirectional(Material material, Light light)
{
    float3 toLight = normalize(light.direction);
	
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseIntensity * mDiffuse;
	
	[flatten]
    if (diffuseIntensity > 0)
    {
        float3 viewDir = normalize(material.worldPos - material.viewPos);
        float3 halfWay = normalize(viewDir + toLight);
        float specular = saturate(dot(material.normal, -halfWay));
		
        finalColor += light.color * pow(specular, shininess)
			* material.specularIntensity * mSpecular;
    }
	
    return finalColor * material.diffuseColor;
}