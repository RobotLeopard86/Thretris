#pragma pack_matrix(row_major)

cbuffer cacao_globals : register(b0) {
  float4x4 projection;
  float4x4 view;
};

cbuffer cacao_locals : register(b1) { float4x4 transform; };

struct VSInput {
  [[vk::location(0)]] float3 Position : POSITION0;
  [[vk::location(1)]] float2 TexCoords : TEXCOORD0;
};

struct VSOutput {
  float4 Pos : SV_POSITION;
  [[vk::location(0)]] float2 TexCoords : TEXCOORD0;
};

VSOutput main(VSInput input) {
  VSOutput output;
  float4 pos = float4(input.Position, 1.0);
  output.Pos = mul(pos, mul(transform, mul(view, projection)));
  output.TexCoords = input.TexCoords;
  return output;
}
