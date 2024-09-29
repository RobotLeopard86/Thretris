#include "texmacro.hlsl"

CacaoTexture(tex, 0);

struct PSInput {
  float4 Pos : SV_POSITION;
  [[vk::location(0)]] float2 TexCoords : TEXCOORD0;
};

struct PSOutput {
  float4 Color : SV_TARGET;
};

PSOutput main(PSInput input) {
  PSOutput output = (PSOutput)0;
  output.Color = tex.Sample(texSampler, input.TexCoords);
  return output;
}
