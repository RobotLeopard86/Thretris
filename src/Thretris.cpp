#include "Thretris.hpp"

#include <cstdlib>
#include <fstream>
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
		hscoreTxt = std::make_shared<Text>();
		hscoreTxt->SetAnchor(AnchorPoint::TopRight);
		hscoreTxt->SetSize({0.2f, 0.05f});
		hscoreTxt->SetText(std::string("High Score: ") + std::to_string(hiScore));
		hscoreTxt->SetAlignment(TextAlign::Right);
		hscoreTxt->SetColor({255.0f, 255.0f, 255.0f});
		hscoreTxt->SetOffsetFromAnchor({0.0f, 0.075f});
		hscoreTxt->SetFont(font);
		hscoreTxt->SetDepth(0);
		hscoreTxt->SetActive(true);
		gameUI->AddElement(hscoreTxt);
		levelTxt = std::make_shared<Text>();
		levelTxt->SetAnchor(AnchorPoint::TopRight);
		levelTxt->SetSize({0.2f, 0.05f});
		levelTxt->SetText("Level 1");
		levelTxt->SetAlignment(TextAlign::Right);
		levelTxt->SetColor({255.0f, 255.0f, 255.0f});
		levelTxt->SetOffsetFromAnchor({0.0f, 0.14f});
		levelTxt->SetFont(font);
		levelTxt->SetDepth(0);
		levelTxt->SetActive(true);
		gameUI->AddElement(levelTxt);

		gameOver = std::make_shared<Screen>();
		gameOverBG = std::make_shared<Image>();
		gameOverBG->SetAnchor(AnchorPoint::Center);
		gameOverBG->SetSize({1.0f, 1.0f});
		gameOverBG->SetImage(gameOverBG_Tex);
		gameOverBG->SetActive(true);
		gameOverBG->SetDepth(1);
		gameOver->AddElement(gameOverBG);
		resetText = std::make_shared<Text>();
		resetText->SetAnchor(AnchorPoint::BottomCenter);
		resetText->SetSize({0.175f, 0.2f});
		resetText->SetText("Press Start To\nPlay Again");
		resetText->SetAlignment(TextAlign::Center);
		resetText->SetColor({255.0f, 255.0f, 255.0f});
		resetText->SetOffsetFromAnchor({-0.05f, -0.1f});
		resetText->SetFont(font);
		resetText->SetDepth(0);
		resetText->SetActive(true);
		gameOver->AddElement(resetText);
		gameOverText = std::make_shared<Text>();
		gameOverText->SetAnchor(AnchorPoint::Center);
		gameOverText->SetSize({0.04f, 0.15f});
		gameOverText->SetOffsetFromAnchor({0.0f, -0.2f});
		gameOverText->SetColor({255.0f, 0.0f, 0.0f});
		gameOverText->SetAlignment(TextAlign::Center);
		gameOverText->SetText("GAME OVER");
		gameOverText->SetActive(true);
		gameOverText->SetFont(font);
		gameOverText->SetDepth(0);
		gameOver->AddElement(gameOverText);
		fscoreText = std::make_shared<Text>();
		fscoreText->SetAnchor(AnchorPoint::Center);
		fscoreText->SetSize({0.1f, 0.1f});
		fscoreText->SetText("Press Start To\nPlay Again");
		fscoreText->SetAlignment(TextAlign::Center);
		fscoreText->SetColor({255.0f, 255.0f, 255.0f});
		fscoreText->SetOffsetFromAnchor({-0.05f, -0.1f});
		fscoreText->SetFont(font);
		fscoreText->SetDepth(0);
		fscoreText->SetActive(true);
		gameOver->AddElement(fscoreText);

		WorldManager::GetInstance()->SetActiveWorld("Game");
		Engine::GetInstance()->GetGlobalUIView()->SetScreen(gameUI);
		Thretris::GetInstance()->musicMaker->Play();
	});
}

void Thretris::IncrementScore() {
	score += floor(pow(1.5, double(level) / 4));
	std::stringstream textStr;
	textStr << "Score: " << score;
	scoreTxt->SetText(textStr.str());
	if(score > hiScore) {
		hiScore = score;
		textStr.str("");
		textStr << "High Score: " << hiScore;
		hscoreTxt->SetText(textStr.str());
	}
}

void Thretris::ResetScore() {
	score = 0;
	scoreTxt->SetText("Score: 0");
}

void Thretris::SetLvl(int lvl) {
	std::stringstream textStr;
	textStr << "Level " << lvl;
	levelTxt->SetText(textStr.str());
	level = lvl;
}

void Thretris::UpdateInfoText(glm::vec3 p, glm::vec3 r) {
	std::stringstream txt;
	txt << "P " << p.x << "/" << p.y << "/" << p.z << " R " << r.x << "/" << r.y << "/" << r.z;
	camInf->SetText(txt.str());
}

void Thretris::OnStartup() {
	std::future<AssetHandle<Texture2D>> menuBgFut = AssetManager::GetInstance()->LoadTexture2D("assets/images/menubg.png");
	std::future<AssetHandle<Texture2D>> overBgFut = AssetManager::GetInstance()->LoadTexture2D("assets/images/gameoverbg.png");
	std::future<AssetHandle<Texture2D>> lgFut = AssetManager::GetInstance()->LoadTexture2D("assets/images/logo.png");
	std::future<AssetHandle<Font>> fontFut = AssetManager::GetInstance()->LoadFont("assets/PixelifySans.ttf");
	std::future<AssetHandle<Sound>> musFut = AssetManager::GetInstance()->LoadSound("assets/gametheme.ogg");

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
	logoTex = lgFut.get();
	logo = std::make_shared<Image>();
	logo->SetAnchor(AnchorPoint::Center);
	logo->SetSize({0.75f, 0.15f});
	logo->SetOffsetFromAnchor({0.0f, -0.2f});
	logo->SetImage(logoTex);
	logo->SetActive(true);
	logo->SetDepth(0);
	mainMenu->AddElement(logo);
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

	gameOverBG_Tex = overBgFut.get();

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

	pub.redM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(red)}, {.target = "InShadow", .data = std::any(0)}}});
	pub.orangeM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(orange)}, {.target = "InShadow", .data = std::any(0)}}});
	pub.goldM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(gold)}, {.target = "InShadow", .data = std::any(0)}}});
	pub.greenM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(green)}, {.target = "InShadow", .data = std::any(0)}}});
	pub.cyanM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(cyan)}, {.target = "InShadow", .data = std::any(0)}}});
	pub.lblueM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(lblue)}, {.target = "InShadow", .data = std::any(0)}}});
	pub.dblueM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(dblue)}, {.target = "InShadow", .data = std::any(0)}}});
	pub.purpleM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(purple)}, {.target = "InShadow", .data = std::any(0)}}});
	pub.pinkM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(pink)}, {.target = "InShadow", .data = std::any(0)}}});

	pub.redM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(red)}, {.target = "InShadow", .data = std::any(1)}}});
	pub.orangeM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(orange)}, {.target = "InShadow", .data = std::any(1)}}});
	pub.goldM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(gold)}, {.target = "InShadow", .data = std::any(1)}}});
	pub.greenM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(green)}, {.target = "InShadow", .data = std::any(1)}}});
	pub.cyanM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(cyan)}, {.target = "InShadow", .data = std::any(1)}}});
	pub.lblueM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(lblue)}, {.target = "InShadow", .data = std::any(1)}}});
	pub.dblueM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(dblue)}, {.target = "InShadow", .data = std::any(1)}}});
	pub.purpleM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(purple)}, {.target = "InShadow", .data = std::any(1)}}});
	pub.pinkM_D = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(pink)}, {.target = "InShadow", .data = std::any(1)}}});

	spaghetti = AssetManager::GetInstance()->LoadSkybox("assets/game.cubedef.yml").get();

	platMsh = AssetManager::GetInstance()->LoadMesh("assets/platform.obj:Platform").get();
	platShd = AssetManager::GetInstance()->LoadShader("assets/shaders/plat.shaderdef.yml").get();
	platMat = std::make_shared<Material>(Material {.shader = platShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(platTex)}}});

	Logging::ClientLog("Generating world...");

	World& world = WorldManager::GetInstance()->GetWorld("Game");
	world.skybox = spaghetti;

	music = musFut.get();
	musicMan = std::make_shared<Entity>("MUUUUUSIC MAN");
	musicMan->SetParent(world.rootEntity);
	musicMan->SetActive(true);
	musicMaker = musicMan->GetComponent<AudioPlayer>(musicMan->MountComponent<AudioPlayer>());
	musicMaker->sound = music;
	musicMaker->SetActive(true);
	musicMaker->SetLooping(true);
	musicMaker->SetGain(1.0f);
	musicMaker->SetPitchMultiplier(1.0f);

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
	platformEnt->GetLocalTransform().SetScale({0.5f, 1.0f, 0.5f});
	std::shared_ptr<MeshComponent> pmc = platformEnt->GetComponent<MeshComponent>(platformEnt->MountComponent<MeshComponent>());
	pmc->mat = platMat;
	pmc->mesh = platMsh;
	pmc->SetActive(true);
	platformEnt->SetParent(world.rootEntity);

	world.cam->SetPosition({-15, 0, 0});

	std::ifstream hsf("./thretris.dat");
	if(!hsf.is_open()) {
		hiScore = 0;
	} else {
		std::stringstream hsc;
		hsc << hsf.rdbuf();
		std::string val = hsc.str();
		Logging::ClientLog(val, LogLevel::Warn);
		try {
			hiScore = std::stoi(val);
		} catch(std::exception) {
			hiScore = 0;
		}
		Logging::ClientLog(std::to_string(hiScore));
		hsf.close();
	}

	Logging::ClientLog("Thretris is started.");
}

void Thretris::OnShutdown() {
	Logging::ClientLog("Saving high-score...");
	std::ofstream hsf("./thretris.dat");
	hsf << hiScore;
	hsf.close();

	Logging::ClientLog("Thretris is stopped.");
}

std::shared_ptr<Material> Thretris::GetShadowVariant(std::shared_ptr<Material> m) {
	if(m == pub.redM) return pub.redM_D;
	if(m == pub.orangeM) return pub.orangeM_D;
	if(m == pub.goldM) return pub.goldM_D;
	if(m == pub.greenM) return pub.greenM_D;
	if(m == pub.cyanM) return pub.cyanM_D;
	if(m == pub.lblueM) return pub.lblueM_D;
	if(m == pub.dblueM) return pub.dblueM_D;
	if(m == pub.purpleM) return pub.purpleM_D;
	if(m == pub.pinkM) return pub.pinkM_D;
	return m;
}

std::shared_ptr<Material> Thretris::GetNormalVariant(std::shared_ptr<Material> m) {
	if(m == pub.redM_D) return pub.redM;
	if(m == pub.orangeM_D) return pub.orangeM;
	if(m == pub.goldM_D) return pub.goldM;
	if(m == pub.greenM_D) return pub.greenM;
	if(m == pub.cyanM_D) return pub.cyanM;
	if(m == pub.lblueM_D) return pub.lblueM;
	if(m == pub.dblueM_D) return pub.dblueM;
	if(m == pub.purpleM_D) return pub.purpleM;
	if(m == pub.pinkM_D) return pub.pinkM;
	return m;
}