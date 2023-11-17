/////////////////////////////////////////////////////////////
// Vertex Shader
/////////////////////////////////////////////////////////////
cbuffer WorldBuffer : register(b0)
{
    matrix world;
}
cbuffer ViewBuffer : register(b1)
{
    matrix view;
    matrix invView; // 원래 ViewMatrix Inverse(카메라위치)
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};
/////////////////////////////////////////////////////////////
// Pixel Shader
/////////////////////////////////////////////////////////////
//Texture & Sampler
SamplerState samp : register(s0);

// Material
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);


struct CubePixelInput
{
    float4 pos : SV_POSITION;
    float3 originPos : POSITION;
};

CubePixelInput VS(VertexUVNormalTangent input)
{
    CubePixelInput ouput;
    ouput.pos.xyz   = mul(input.pos.xyz, (float3x3) view);
    ouput.pos.w     = 1.0f;
    
    ouput.pos       = mul(ouput.pos, projection);
    ouput.originPos = input.pos.xyz;
    return ouput;
 
}
TextureCube cubeMap : register(t10);
float4 PS(CubePixelInput input) : SV_Target
{
    return cubeMap.Sample(samp, input.originPos);

}