#include "texmacro.hlsl"

CacaoTexture(tex, 0);

struct PSInput {
  float4 Pos : SV_POSITION;
  [[vk::location(0)]] float2 TexCoords : TEXCOORD;
  [[vk::location(1)]] bool Shadowed : Shadows;
};

struct PSOutput {
  float4 Color : SV_TARGET;
};

PSOutput main(PSInput input) {
  PSOutput output = (PSOutput)0;
  float2 tc = (input.TexCoords / 2) - 0.5;
  output.Color = tex.Sample(texSampler, tc);
  if (input.Shadowed)
    output.Color *= 0.8;
  return output;
}
