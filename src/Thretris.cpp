#include "Thretris.hpp"

#include <cstdlib>
#include <cstddef>

Thretris* Thretris::instance = nullptr;
bool Thretris::instanceExists = false;

Thretris* Thretris::GetInstance() {
	//Do we have an instance yet?
	if(!instanceExists || instance == NULL) {
		//Create instance
		instance = new Thretris();
		instanceExists = true;
	}

	return instance;
}

void Thretris::OnStartup() {
	Logging::ClientLog("Loading Thretris assets...");

	std::future<AssetHandle<Texture2D>> menuBgFut = AssetManager::GetInstance()->LoadTexture2D("assets/images/menubg.png");
	std::future<AssetHandle<Mesh>> blockFut = AssetManager::GetInstance()->LoadMesh("assets/blocks/block.obj:Block");
	std::future<AssetHandle<Shader>> blockShaderFut = AssetManager::GetInstance()->LoadShader("assets/shaders/block.shaderdef.yml");
	std::future<AssetHandle<Texture2D>> goldTexFut = AssetManager::GetInstance()->LoadTexture2D("assets/blocks/gold.png");

	WorldManager::GetInstance()->CreateWorld<PerspectiveCamera>("MainMenu");
	WorldManager::GetInstance()->SetActiveWorld("MainMenu");

	mainMenu = std::make_shared<Screen>();
	mainMenuBG_Tex = menuBgFut.get();
	mainMenuBG = std::make_shared<Image>();
	mainMenuBG->SetAnchor(AnchorPoint::Center);
	mainMenuBG->SetSize({1.0f, 1.0f});
	mainMenuBG->SetImage(mainMenuBG_Tex);
	mainMenuBG->SetActive(true);
	mainMenu->AddElement(mainMenuBG);
	//Engine::GetInstance()->GetGlobalUIView()->SetScreen(mainMenu);

	block = blockFut.get();
	blockShader = blockShaderFut.get();
	goldTex = goldTexFut.get();
	goldMat = std::make_shared<Material>();
	goldMat->data.emplace_back(ShaderUploadItem {.target = "tex", .data = std::any(goldTex)});
	goldMat->shader = blockShader;

	goldBlock = std::make_shared<Entity>("Gold Block");
	goldBlock->SetActive(true);
	goldBlock->SetParent(WorldManager::GetInstance()->GetActiveWorld().rootEntity);
	goldBlock->GetLocalTransform().SetPosition({0, 0, -5});
	std::shared_ptr<MeshComponent> gbmc = goldBlock->GetComponent<MeshComponent>(goldBlock->MountComponent<MeshComponent>());
	gbmc->SetActive(true);
	gbmc->mat = goldMat;
	gbmc->mesh = block;

	Logging::ClientLog("Thretris is started.");
}

void Thretris::OnShutdown() {
	Logging::ClientLog("Thretris is stopped.");
}
