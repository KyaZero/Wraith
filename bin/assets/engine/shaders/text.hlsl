
#include "common.hlsl"

Texture2D<float3> AtlasTexture : register(t0);
SamplerState DefaultSampler : register(s0);

cbuffer BufferData : register(b0)
{
    float4x4 Projection;
    float PixelRange;
    float FontSize;
    float AtlasSize;
    float _Padding;
};

struct InstanceBuffer
{
    float2 UvOffset;
    float2 UvScale;
    float2 Offset;
    float2 Position;
    float FontScale;
    float _Padding;
};
StructuredBuffer<InstanceBuffer> InstanceData : register(t1);

void VSMain(in VertexInput input, out PixelInput output, uint instance_ID : SV_InstanceID)
{
    const float2 uv_scale = InstanceData[instance_ID].UvScale;
    const float2 uv_offset = InstanceData[instance_ID].UvOffset;
    const float2 offset = InstanceData[instance_ID].Offset;
    const float2 position = InstanceData[instance_ID].Position;
    const float font_scale = InstanceData[instance_ID].FontScale;

    float2 p = input.position.xy;
    p *= uv_scale;
    p += offset;
    p *= AtlasSize / FontSize;
    p += position;

    p *= FontSize * font_scale;

    output.position = mul(Projection, float4(p, 0.0, 1.0));
    output.uv = input.uv * uv_scale + uv_offset;
}

float screenPxRange(float2 texcoord)
{
    const float2 unitRange = PixelRange / AtlasSize;
    const float2 screenTexSize = 1.0 / fwidth(texcoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void PSMain(in PixelInput input, out PixelOutput output)
{
    const float3 msd = AtlasTexture.Sample(DefaultSampler, input.uv).rgb;
    const float sd = max(min(msd.r, msd.g), min(max(msd.r, msd.g), msd.b));
    const float screenPxDistance = screenPxRange(input.uv) * (sd - 0.5);
    const float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    output.color.rgba = opacity;
}