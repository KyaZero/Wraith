#include "common.hlsl"

Texture2D ColorTexture : register(t0);
SamplerState DefaultSampler : register(s0);

struct VertexInput
{
    float2 position : POSITION;
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

cbuffer BufferData : register(b0)
{
    float4x4 ViewProjection;
    float4x4 Projection;
    float2 Resolution;
}

struct InstanceBuffer
{
    float4 Color;
    float2 Position;
    float2 Offset;
    float2 Scale;
    float2 Size;
    float Rotation;
    int WorldSpace;
};
StructuredBuffer<InstanceBuffer> InstanceData : register(t1);

float2 Rotate(float rotation, float2 position)
{
    float c = cos(rotation);
    float s = sin(rotation);
    float2x2 r = float2x2(c, -s, s, c);
    return mul(position, r);
}

void VSMain(in VertexInput input, out PixelInput output, uint instance_ID : SV_InstanceID)
{
    input.position *= InstanceData[instance_ID].Size;
    input.position -= InstanceData[instance_ID].Offset;
    input.position = Rotate(InstanceData[instance_ID].Rotation, input.position);
    input.position *= InstanceData[instance_ID].Scale;
    input.position += InstanceData[instance_ID].Position;

    output.position = InstanceData[instance_ID].WorldSpace ? mul(ViewProjection, float4(input.position.xy, 0, 1))
                                                           : mul(Projection, float4(input.position.xy, 0, 1));
    output.color = InstanceData[instance_ID].Color;
    output.uv = input.uv;
}

void PSMain(in PixelInput input, out PixelOutput output)
{
    float4 color = ColorTexture.Sample(DefaultSampler, input.uv);
    color.rgb = TO_LINEAR(color.rgb);

    float4 final_color = color;
    final_color *= input.color;
    final_color.rgb = TO_SRGB(final_color.rgb);

    output.color = final_color;
}