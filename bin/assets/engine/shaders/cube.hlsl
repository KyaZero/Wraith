#include "common.hlsli"

Texture2D Texture : register(t0);
SamplerState DefaultSampler : register(s0);

struct VertexInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BINORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct PixelOutput
{
    float4 color : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
    float4x4 projection;
    float4x4 view;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 transform;
}

void VSMain(in VertexInput input, out PixelInput output)
{
    float4 world_pos = mul(transform, input.position);
    float4 view_pos = mul(view, world_pos);
    float4 proj_pos = mul(projection, view_pos);

    output.position = proj_pos;
    output.color = input.color;
    output.uv = input.uv;
}

void PSMain(in PixelInput input, out PixelOutput output)
{
    output.color = TO_SRGB(input.color * TO_LINEAR(Texture.Sample(DefaultSampler, input.uv)));
}