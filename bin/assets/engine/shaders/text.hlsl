
#include "common.hlsl"

Texture2D<float3> AtlasTexture : register(t0);
SamplerState DefaultSampler : register(s0);

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void VSMain(in VertexInput input, out PixelInput output, uint instance_ID : SV_InstanceID)
{
    output.position = float4(input.position.xy*2-1, 0.0, 1.0);
    output.uv = input.uv;
}

float screenPxRange(float2 texcoord) {
    float pxRange = 2.0;
    float2 unitRange = pxRange/256.0;
    float2 screenTexSize = 1.0/fwidth(texcoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void PSMain(in PixelInput input, out PixelOutput output)
{
    float3 msd = AtlasTexture.Sample(DefaultSampler, input.uv).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange(input.uv)*(sd-0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    output.color.rgb = opacity;
    output.color.a = 1.0;
}