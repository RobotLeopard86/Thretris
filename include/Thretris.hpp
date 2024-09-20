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
	std::shared_ptr<Material> redM_D;
	std::shared_ptr<Material> orangeM_D;
	std::shared_ptr<Material> goldM_D;
	std::shared_ptr<Material> greenM_D;
	std::shared_ptr<Material> cyanM_D;
	std::shared_ptr<Material> lblueM_D;
	std::shared_ptr<Material> dblueM_D;
	std::shared_ptr<Material> purpleM_D;
	std::shared_ptr<Material> pinkM_D;
	AssetHandle<Mesh> block;
};

class Thretris {
  public:
	static Thretris* GetInstance();

	void OnStartup();
	void OnShutdown();

	void DoStart();

	void UpdateInfoText(glm::vec3 p, glm::vec3 r, glm::vec3 o);
	void IncrementScore();
	void ResetScore();
	void SetLvl(int lvl);
	int GetLvl() {
		return level;
	}

	void GameOver();
	void ShowGameUI() {
		Engine::GetInstance()->GetGlobalUIView()->SetScreen(gameUI);
	}
	void ShowPauseUI() {
		Engine::GetInstance()->GetGlobalUIView()->SetScreen(pause);
	}
	void ToggleHowToPlay();

	bool IsPaused() {
		return Engine::GetInstance()->GetGlobalUIView()->GetScreen() == pause;
	}

	std::shared_ptr<Material> GetNormalVariant(std::shared_ptr<Material> m);
	std::shared_ptr<Material> GetShadowVariant(std::shared_ptr<Material> m);

	PublicAssets pub;
	std::array<std::array<std::array<std::shared_ptr<Entity>, 20>, 10>, 10> blks;
	std::shared_ptr<AudioPlayer> musicMaker;

  private:
	static Thretris* instance;
	static bool instanceExists;

	AssetHandle<Font> font;

	std::shared_ptr<Entity> menuMgr;

	std::shared_ptr<Screen> mainMenu;
	std::shared_ptr<Screen> gameUI;
	std::shared_ptr<Screen> gameOver;
	std::shared_ptr<Screen> pause;

	AssetHandle<Texture2D> mainMenuBG_Tex;
	std::shared_ptr<Image> mainMenuBG;
	std::shared_ptr<Text> startText;
	AssetHandle<Texture2D> logoTex;
	std::shared_ptr<Image> logo;

	AssetHandle<Texture2D> gameOverBG_Tex;
	std::shared_ptr<Image> gameOverBG;
	std::shared_ptr<Text> gameOverText;
	std::shared_ptr<Text> resetText;
	std::shared_ptr<Text> fscoreText;

	std::shared_ptr<Text> camInfP;
	std::shared_ptr<Text> camInfR;
	std::shared_ptr<Text> camInfO;

	std::shared_ptr<Text> scoreTxt;
	std::shared_ptr<Text> hscoreTxt;
	int score;
	int hiScore;
	std::shared_ptr<Text> levelTxt;
	int level;

	std::shared_ptr<Text> pauseInstructions;
	std::shared_ptr<Text> how2Play;
	AssetHandle<Texture2D> pauseBG_Tex;
	std::shared_ptr<Image> pauseBG;

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

	AssetHandle<Sound> music;
	std::shared_ptr<Entity> musicMan;

	AssetHandle<Skybox> spaghetti;

	std::shared_ptr<Entity> camMgrEnt;

	std::shared_ptr<Entity> platformEnt;
	AssetHandle<Shader> platShd;
	AssetHandle<Texture2D> platTex;
	AssetHandle<Mesh> platMsh;
	std::shared_ptr<Material> platMat;
};
