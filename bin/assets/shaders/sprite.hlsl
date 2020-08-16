#include "common.hlsl"

Texture2D ColorTexture : register(t0);
SamplerState DefaultSampler : register(s0);

void VSMain(in VertexInput input, out GeometryInput output)
{
    output.position = input.position;
    output.uv = input.uv;
    output.color = float4(input.color.rgb, 1.0f) * input.color.a;
}

[maxvertexcount(4)]
void GSMain(point GeometryInput input[1], inout TriangleStream<PixelInput> output)
{
    GeometryInput pt = input[0];

    float2 size = 0.5f * Size;
    const float4 offset[4] = 
    {
        { -size.x,  size.y, 0, 0 },
        {  size.x,  size.y, 0, 0 },
        { -size.x, -size.y, 0, 0 },
        {  size.x, -size.y, 0, 0 },
    };

    const float2 uv[4] = 
    {
        {pt.uv.x, pt.uv.y},
        {pt.uv.x + pt.uv.z, pt.uv.y},
        {pt.uv.x, pt.uv.y + pt.uv.w},
        {pt.uv.x + pt.uv.z, pt.uv.y + pt.uv.w},
    };

    const float2 duv = pt.uv.zw;

    PixelInput vertex;
    for (int i = 0; i < 4; ++i)
    {
        vertex.position = pt.position;
        vertex.position.xy += offset[i].xy * duv;
        vertex.position = mul(ProjectionMatrix, vertex.position);

        vertex.uv = uv[i];
        vertex.color = pt.color;

        output.Append(vertex);
    }

    output.RestartStrip();
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