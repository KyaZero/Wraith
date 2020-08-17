#include "common.hlsl"

Texture2D ColorTexture : register(t0);
SamplerState DefaultSampler : register(s0);

float2 Rotate(float rotation, float2 position)
{
	float c = cos(rotation);
	float s = sin(rotation);
	float2x2 r = float2x2(c, -s, s, c);
	return mul(position, r);
}

void VSMain(in VertexInput input, out PixelInput output)
{
    input.position *= Size;
    input.position -= Offset;
    input.position = Rotate(Rotation, input.position);
    input.position *= Scale;
    input.position += Position;

    output.position = mul(ViewProjection, float4(input.position.xy, 0, 1));
    output.uv = input.uv;
}

void PSMain(in PixelInput input, out PixelOutput output)
{
    float4 color = ColorTexture.Sample(DefaultSampler, input.uv);
    color.rgb = TO_LINEAR(color.rgb);

    float4 final_color = color;
    final_color *= Color;
    final_color.rgb = TO_SRGB(final_color.rgb);

    output.color = final_color;
}