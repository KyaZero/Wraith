struct VertexInput
{
    float2 position : POSITION;
    float2 uv : TEXCOORD;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct PixelOutput
{
    float4 color : SV_TARGET;
};

cbuffer BufferData : register(b0)
{
    float4x4 Projection;
    float4 Color;
    float2 Position;
    float2 Offset;
    float2 Scale;
    float2 Size;
    float2 Resolution;
    float Rotation;
    float Time;
}

#define TO_SRGB(_Linear_Color) (pow(_Linear_Color, 1.f / 2.2f))
#define TO_LINEAR(_SRGB_Color) (pow(_SRGB_Color, 2.2f))