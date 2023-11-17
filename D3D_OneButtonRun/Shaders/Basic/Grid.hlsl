// Vertex
cbuffer WorldBuffer       : register(b0)
{
    matrix world;
}
cbuffer ViewBuffer       : register(b1)
{
    matrix view;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

//Vertex Layout
struct VertexUV
{
    float4 pos : POSITION;
    float2 uv  : UV;
};
struct VertexColor
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

// shader program

//시멘틱(Sementic) : 반환값이나 매개변수가 어떻게 사용할지 나타내는 것
//SV(SystemValue)
PixelInput VS(VertexColor input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.color = input.color;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return input.color;
}


