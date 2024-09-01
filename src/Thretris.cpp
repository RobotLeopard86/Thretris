#include "Thretris.hpp"

#include <cstdlib>
#include <cstddef>

#include "CamMgr.hpp"
#include "RandomStuff.hpp"
#include "Thretromino.hpp"
#include "GameMgr.hpp"

Thretris* Thretris::instance = nullptr;
bool Thretris::instanceExists = false;

Thretris* Thretris::GetInstance() {
	//Do we have an instance yet?
	if(!instanceExists || instance == nullptr) {
		//Create instance
		instance = new Thretris();
		instanceExists = true;
	}

	return instance;
}

void Thretris::DoStart() {
	Engine::GetInstance()->GetThreadPool()->enqueue_detach([this]() {
		Logging::ClientLog("Let's go!");

		gameUI = std::make_shared<Screen>();
		camInf = std::make_shared<Text>();
		camInf->SetAnchor(AnchorPoint::TopLeft);
		camInf->SetSize({0.4f, 0.05f});
		camInf->SetText("P X/Y/Z R X/Y/Z");
		camInf->SetAlignment(TextAlign::Left);
		camInf->SetColor({255.0f, 255.0f, 255.0f});
		camInf->SetOffsetFromAnchor({0.0f, 0.01f});
		camInf->SetFont(font);
		camInf->SetDepth(0);
		camInf->SetActive(true);
		gameUI->AddElement(camInf);
		scoreTxt = std::make_shared<Text>();
		scoreTxt->SetAnchor(AnchorPoint::TopRight);
		scoreTxt->SetSize({0.2f, 0.05f});
		scoreTxt->SetText("Score: 0");
		scoreTxt->SetAlignment(TextAlign::Right);
		scoreTxt->SetColor({255.0f, 255.0f, 255.0f});
		scoreTxt->SetOffsetFromAnchor({0.0f, 0.01f});
		scoreTxt->SetFont(font);
		scoreTxt->SetDepth(0);
		scoreTxt->SetActive(true);
		gameUI->AddElement(scoreTxt);
		levelTxt = std::make_shared<Text>();
		levelTxt->SetAnchor(AnchorPoint::TopRight);
		levelTxt->SetSize({0.2f, 0.05f});
		levelTxt->SetText("Level 1");
		levelTxt->SetAlignment(TextAlign::Right);
		levelTxt->SetColor({255.0f, 255.0f, 255.0f});
		levelTxt->SetOffsetFromAnchor({0.0f, 0.075f});
		levelTxt->SetFont(font);
		levelTxt->SetDepth(0);
		levelTxt->SetActive(true);
		gameUI->AddElement(levelTxt);

		WorldManager::GetInstance()->SetActiveWorld("Game");
		Engine::GetInstance()->GetGlobalUIView()->SetScreen(gameUI);
	});
}

void Thretris::IncrementScore() {
	score++;
	std::stringstream textStr;
	textStr << "Score: " << score;
	scoreTxt->SetText(textStr.str());
}

void Thretris::SetLvl(int lvl) {
	std::stringstream textStr;
	textStr << "Level " << lvl;
	levelTxt->SetText(textStr.str());
}

void Thretris::UpdateInfoText(glm::vec3 p, glm::vec3 r) {
	std::stringstream txt;
	txt << "P " << p.x << "/" << p.y << "/" << p.z << " R " << r.x << "/" << r.y << "/" << r.z;
	camInf->SetText(txt.str());
}

void Thretris::OnStartup() {
	std::future<AssetHandle<Texture2D>> menuBgFut = AssetManager::GetInstance()->LoadTexture2D("assets/images/menubg.png");
	std::future<AssetHandle<Font>> fontFut = AssetManager::GetInstance()->LoadFont("assets/PixelifySans.ttf");

	WorldManager::GetInstance()->CreateWorld<PerspectiveCamera>("MainMenu");
	WorldManager::GetInstance()->CreateWorld<PerspectiveCamera>("Game");
	WorldManager::GetInstance()->SetActiveWorld("MainMenu");

	mainMenu = std::make_shared<Screen>();
	mainMenuBG_Tex = menuBgFut.get();
	mainMenuBG = std::make_shared<Image>();
	mainMenuBG->SetAnchor(AnchorPoint::Center);
	mainMenuBG->SetSize({1.0f, 1.0f});
	mainMenuBG->SetImage(mainMenuBG_Tex);
	mainMenuBG->SetActive(true);
	mainMenuBG->SetDepth(1);
	mainMenu->AddElement(mainMenuBG);
	startText = std::make_shared<Text>();
	startText->SetAnchor(AnchorPoint::BottomCenter);
	startText->SetSize({0.1f, 0.1f});
	startText->SetText("Press Start");
	startText->SetAlignment(TextAlign::Center);
	startText->SetColor({255.0f, 255.0f, 255.0f});
	startText->SetOffsetFromAnchor({-0.05f, -0.1f});
	font = fontFut.get();
	startText->SetFont(font);
	startText->SetDepth(0);
	startText->SetActive(true);
	mainMenu->AddElement(startText);
	Engine::GetInstance()->GetGlobalUIView()->SetScreen(mainMenu);

	menuMgr = std::make_shared<Entity>("Menu Manager");
	menuMgr->SetParent(WorldManager::GetInstance()->GetActiveWorld().rootEntity);
	menuMgr->SetActive(true);
	menuMgr->GetComponent<GameStarter>(menuMgr->MountComponent<GameStarter>())->SetActive(true);
	menuMgr->GetComponent<ExitHandler>(menuMgr->MountComponent<ExitHandler>())->SetActive(true);
	std::shared_ptr<Dirtifier> dirtifier = menuMgr->GetComponent<Dirtifier>(menuMgr->MountComponent<Dirtifier>());
	dirtifier->SetActive(true);

	MultiFuture<AssetHandle<Texture2D>> texLoadOp;
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/red.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/orange.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/gold.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/green.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/cyan.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/lightblue.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/darkblue.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/purple.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/blocks/pink.png"));
	texLoadOp.push_back(AssetManager::GetInstance()->LoadTexture2D("assets/images/plat.png"));

	pub.block = AssetManager::GetInstance()->LoadMesh("assets/blocks/block.obj:Block").get();
	blockShd = AssetManager::GetInstance()->LoadShader("assets/shaders/block.shaderdef.yml").get();

	red = texLoadOp[0].get();
	orange = texLoadOp[1].get();
	gold = texLoadOp[2].get();
	green = texLoadOp[3].get();
	cyan = texLoadOp[4].get();
	lblue = texLoadOp[5].get();
	dblue = texLoadOp[6].get();
	purple = texLoadOp[7].get();
	pink = texLoadOp[8].get();
	platTex = texLoadOp[9].get();

	pub.redM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(red)}}});
	pub.orangeM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(orange)}}});
	pub.goldM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(gold)}}});
	pub.greenM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(green)}}});
	pub.cyanM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(cyan)}}});
	pub.lblueM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(lblue)}}});
	pub.dblueM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(dblue)}}});
	pub.purpleM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(purple)}}});
	pub.pinkM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(pink)}}});

	spaghetti = AssetManager::GetInstance()->LoadSkybox("assets/game.cubedef.yml").get();

	platMsh = AssetManager::GetInstance()->LoadMesh("assets/platform.obj:Platform").get();
	platShd = AssetManager::GetInstance()->LoadShader("assets/shaders/plat.shaderdef.yml").get();
	platMat = std::make_shared<Material>(Material {.shader = platShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(platTex)}}});

	Logging::ClientLog("Generating world...");

	World& world = WorldManager::GetInstance()->GetWorld("Game");
	world.skybox = spaghetti;

	score = 0;
	level = 1;

	camMgrEnt = std::make_shared<Entity>("Thetris Manager");
	camMgrEnt->SetParent(world.rootEntity);
	camMgrEnt->SetActive(true);
	camMgrEnt->GetComponent<CamMgr>(camMgrEnt->MountComponent<CamMgr>())->SetActive(true);
	camMgrEnt->GetComponent<GameMgr>(camMgrEnt->MountComponent<GameMgr>())->SetActive(true);
	camMgrEnt->GetComponent<ExitHandler>(camMgrEnt->MountComponent<ExitHandler>())->SetActive(true);

	platformEnt = std::make_shared<Entity>("Floor Platform");
	platformEnt->SetActive(true);
	platformEnt->GetLocalTransform().SetPosition({9.5625f, -11, -0.5f});
	platformEnt->GetLocalTransform().SetScale({0.5f, 1, 0.5f});
	std::shared_ptr<MeshComponent> pmc = platformEnt->GetComponent<MeshComponent>(platformEnt->MountComponent<MeshComponent>());
	pmc->mat = platMat;
	pmc->mesh = platMsh;
	pmc->SetActive(true);
	platformEnt->SetParent(world.rootEntity);

	world.cam->SetPosition({-15, 0, 0});

	Logging::ClientLog("Thretris is started.");
}

void Thretris::OnShutdown() {
	Logging::ClientLog("Thretris is stopped.");
}
