#define CacaoTexture(name, reg)                                                \
  [[vk::binding(reg + 1)]] [[vk::combinedImageSampler]] Texture2D name         \
      : register(t##reg);                                                      \
  [[vk::binding(reg +                                                          \
                1)]] [[vk::combinedImageSampler]] SamplerState name##Sampler   \
      : register(s##reg)
