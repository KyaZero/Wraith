struct VertexInput
{
    float4 position : POSITION;
    float4 uv : TEXCOORD;
    float4 color : COLOR;
};

struct GeometryInput
{
    float4 position : POSITION;
    float4 uv : TEXCOORD;
    float4 color : COLOR;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct PixelOutput
{
    float4 color : SV_TARGET;
};

cbuffer BufferData : register(b0)
{
    float4x4 ProjectionMatrix;
    float4 Resolution;
    float2 Time;
    float2 Size;
}

#define TO_SRGB(_Linear_Color) (pow(_Linear_Color, 1.f / 2.2f))
#define TO_LINEAR(_SRGB_Color) (pow(_SRGB_Color, 2.2f))