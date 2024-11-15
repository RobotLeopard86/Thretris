#pragma pack_matrix(row_major)

struct CacaoGlobals {
  float4x4 projection;
  float4x4 view;
};

ConstantBuffer<CacaoGlobals> globals : register(b0);

struct VSInput {
  [[vk::location(0)]] float3 Position : POSITION0;
  [[vk::location(1)]] float2 TexCoords : TEXCOORD0;
};

struct VSOutput {
  float4 Pos : SV_POSITION;
  [[vk::location(0)]] float2 TexCoords : TEXCOORD;
};

struct ObjectData {
  float4x4 transform;
};

[[vk::push_constant]] ObjectData object;

VSOutput main(VSInput input) {
  VSOutput output;
  float4 pos = float4(input.Position, 1.0);
  output.Pos =
      mul(pos, mul(object.transform, mul(globals.view, globals.projection)));
  output.TexCoords = input.TexCoords;
  return output;
}
