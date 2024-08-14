#pragma once

#include "Cacao.hpp"

using namespace Cacao;

#include "Thretromino.hpp"

struct PublicAssets {
	std::shared_ptr<Material> redM;
	std::shared_ptr<Material> orangeM;
	std::shared_ptr<Material> goldM;
	std::shared_ptr<Material> greenM;
	std::shared_ptr<Material> cyanM;
	std::shared_ptr<Material> lblueM;
	std::shared_ptr<Material> dblueM;
	std::shared_ptr<Material> purpleM;
	std::shared_ptr<Material> pinkM;
	AssetHandle<Mesh> block;
};

class Thretris {
  public:
	static Thretris* GetInstance();

	void OnStartup();
	void OnShutdown();

	void DoStart();

	void UpdateInfoText(glm::vec3 p, glm::vec3 r);

	PublicAssets pub;
	std::array<std::array<std::array<std::shared_ptr<Entity>, 20>, 10>, 10> blks;

  private:
	static Thretris* instance;
	static bool instanceExists;

	AssetHandle<Font> font;

	std::shared_ptr<Entity> menuMgr;

	std::shared_ptr<Screen> mainMenu;
	std::shared_ptr<Screen> gameUI;

	AssetHandle<Texture2D> mainMenuBG_Tex;
	std::shared_ptr<Image> mainMenuBG;
	std::shared_ptr<Text> startText;

	std::shared_ptr<Text> camInf;

	AssetHandle<Texture2D> red;
	AssetHandle<Texture2D> orange;
	AssetHandle<Texture2D> gold;
	AssetHandle<Texture2D> green;
	AssetHandle<Texture2D> cyan;
	AssetHandle<Texture2D> lblue;
	AssetHandle<Texture2D> dblue;
	AssetHandle<Texture2D> purple;
	AssetHandle<Texture2D> pink;
	AssetHandle<Shader> blockShd;

	AssetHandle<Skybox> spaghetti;

	std::shared_ptr<Entity> camMgrEnt;

	std::shared_ptr<Entity> platformEnt;
	AssetHandle<Shader> platShd;
	AssetHandle<Texture2D> platTex;
	AssetHandle<Mesh> platMsh;
	std::shared_ptr<Material> platMat;
};
