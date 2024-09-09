#pragma pack_matrix(row_major)

struct CacaoGlobals {
  float4x4 projection;
  float4x4 view;
};

struct CacaoLocals {
  float4x4 transform;
};

ConstantBuffer<CacaoGlobals> globals : register(b0);
ConstantBuffer<CacaoLocals> locals : register(b1);

struct VSInput {
  [[vk::location(0)]] float3 Position : POSITION0;
  [[vk::location(1)]] float2 TexCoords : TEXCOORD0;
};

struct VSOutput {
  float4 Pos : SV_POSITION;
  [[vk::location(0)]] float2 TexCoords : TEXCOORD;
  [[vk::location(1)]] bool Shadowed : Shadows;
};

struct ShaderData {
  int InShadow;
};

[[vk::push_constant]] ShaderData shader;

VSOutput main(VSInput input) {
  VSOutput output;
  float4 pos = float4(input.Position, 1.0);
  output.Pos =
      mul(pos, mul(locals.transform, mul(globals.view, globals.projection)));
  output.TexCoords = input.TexCoords;
  output.Shadowed = shader.InShadow > 0 ? true : false;
  return output;
}
