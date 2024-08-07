[[vk::combinedImageSampler]] Texture2D tex : register(t0);
[[vk::combinedImageSampler]] SamplerState texSampler : register(s0);

struct PSInput {
  float4 Pos : SV_POSITION;
  [[vk::location(0)]] float2 TexCoords : TEXCOORD0;
};

struct PSOutput {
  float4 Color : SV_TARGET;
};

PSOutput main(PSInput input) {
  PSOutput output = (PSOutput)0;
  float2 tc = input.TexCoords / 2;
  output.Color = tex.Sample(texSampler, tc);
  return output;
}