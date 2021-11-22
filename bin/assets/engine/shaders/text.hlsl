#include "common.hlsli"

Texture2D<float3> AtlasTexture : register(t0);
SamplerState LinearSampler : register(s0);

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

cbuffer BufferData : register(b0) {
  float4x4 Projection;
  float PixelRange;
  float FontSize;
  float AtlasSize;
  float _Padding;
};

struct InstanceBuffer {
  float4 TextColor;
  float2 GlyphUvOffset;
  float2 GlyphUvSize;
  float2 BaselineOffset;
  float2 CharacterPosition;
  float2 ScreenPosition;
  float TextScale;
  float BlendMode;
};
StructuredBuffer<InstanceBuffer> InstanceData : register(t1);

void VSMain(in VertexInput input, out PixelInput output, uint instance_ID
            : SV_InstanceID) {
  const float4 text_color = InstanceData[instance_ID].TextColor;
  const float2 glyph_uv_offset = InstanceData[instance_ID].GlyphUvOffset;
  const float2 glyph_uv_size = InstanceData[instance_ID].GlyphUvSize;
  const float2 baseline_offset = InstanceData[instance_ID].BaselineOffset;
  const float2 character_position = InstanceData[instance_ID].CharacterPosition;
  const float2 screen_position = InstanceData[instance_ID].ScreenPosition;
  const float text_scale = InstanceData[instance_ID].TextScale;
  const float blend_mode = InstanceData[instance_ID].BlendMode;

  // Order of operations is important here:
  // 1. Move the uv to the glyph in the atlas
  float2 p = input.position.xy;
  // 2. Scale the uvs to cover the whole glyph
  p *= glyph_uv_size;
  // 3. Translate the position so it lands on the baseline
  p += baseline_offset;
  // 4. Scale the position to make it independent of the atlas size
  p *= AtlasSize / FontSize;
  // 5. Translate the position in relation to the text it is part of
  p += character_position;
  // 6. Scale the position according to the text scale
  p *= text_scale;
  // 7. Translate the position i screenspace in pixels
  p += screen_position;

  output.position = mul(Projection, float4(p, 0.0, 1.0));
  output.uv = input.uv * glyph_uv_size + glyph_uv_offset;
  output.color =
      float4(text_color.rgb * text_color.a, text_color.a * blend_mode);
}

float ScreenPxRange(float2 texcoord) {
  const float2 unit_range = PixelRange / AtlasSize;
  const float2 screen_tex_size = 1.0 / fwidth(texcoord);
  return max(0.5 * dot(unit_range, screen_tex_size), 1.0);
}

void PSMain(in PixelInput input, out PixelOutput output) {
  const float3 msd = AtlasTexture.Sample(LinearSampler, input.uv).rgb;
  const float sd = max(min(msd.r, msd.g), min(max(msd.r, msd.g), msd.b));
  const float screen_px_distance = ScreenPxRange(input.uv) * (sd - 0.5);
  const float opacity = clamp(screen_px_distance + 0.5, 0.0, 1.0);

  output.color.rgba = opacity;
  output.color *= input.color;
}
