#pragma once

#include "Cacao.hpp"

using namespace Cacao;

class Thretris {
  public:
	static Thretris* GetInstance();

	void OnStartup();
	void OnShutdown();

  private:
	static Thretris* instance;
	static bool instanceExists;

	std::shared_ptr<Screen> mainMenu;

	AssetHandle<Texture2D> mainMenuBG_Tex;
	std::shared_ptr<Image> mainMenuBG;

	AssetHandle<Mesh> block;
	AssetHandle<Shader> blockShader;

	AssetHandle<Texture2D> goldTex;
	std::shared_ptr<Material> goldMat;

	std::shared_ptr<Entity> goldBlock;
};
