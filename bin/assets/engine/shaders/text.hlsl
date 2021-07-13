
#include "common.hlsl"

Texture2D<float3> AtlasTexture : register(t0);
SamplerState DefaultSampler : register(s0);

cbuffer BufferData : register(b0)
{
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
};
StructuredBuffer<InstanceBuffer> InstanceData : register(t1);

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void VSMain(in VertexInput input, out PixelInput output, uint instance_ID : SV_InstanceID)
{
    const float2 uv_scale = InstanceData[instance_ID].UvScale;
    const float2 uv_offset = InstanceData[instance_ID].UvOffset;
    const float2 offset = InstanceData[instance_ID].Offset;
    const float2 position = InstanceData[instance_ID].Position;

    output.position = float4(input.position.xy, 0.0, 1.0);
    output.position.xy *= uv_scale;
    output.position.xy += offset;
    output.position.xy += position;
    output.position.xy *= AtlasSize / FontSize;

    output.uv = input.uv * uv_scale + uv_offset;
}

float screenPxRange(float2 texcoord) {
    float2 unitRange = PixelRange/(AtlasSize*2.0);
    float2 screenTexSize = 1.0/fwidth(texcoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void PSMain(in PixelInput input, out PixelOutput output)
{
    float3 msd = AtlasTexture.Sample(DefaultSampler, input.uv).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange(input.uv)*(sd-0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    output.color.rgba = opacity;
}