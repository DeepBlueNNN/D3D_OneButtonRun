#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv  : UV;
};

// shader program

//시멘틱(Sementic) : 반환값이나 매개변수가 어떻게 사용할지 나타내는 것
//SV(SystemValue)
PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv  = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return diffuseMap.Sample(samp, input.uv);
}
