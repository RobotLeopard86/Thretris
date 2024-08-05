#include "Thretris.hpp"

#include <cstdlib>
#include <cstddef>

#include "CamMgr.hpp"
#include "RandomScripts.hpp"

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

void Thretris::DoStart() {
	Engine::GetInstance()->GetThreadPool()->enqueue_detach([this]() {
		Logging::ClientLog("Let's go!");

		gameUI = std::make_shared<Screen>();
		WorldManager::GetInstance()->SetActiveWorld("Game");
		Engine::GetInstance()->GetGlobalUIView()->SetScreen(gameUI);
	});
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

	block = AssetManager::GetInstance()->LoadMesh("assets/blocks/block.obj:Block").get();
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

	redM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(red)}}});
	orangeM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(orange)}}});
	goldM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(gold)}}});
	greenM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(green)}}});
	cyanM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(cyan)}}});
	lblueM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(lblue)}}});
	dblueM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(dblue)}}});
	purpleM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(purple)}}});
	pinkM = std::make_shared<Material>(Material {.shader = blockShd, .data = ShaderUploadData {{.target = "tex", .data = std::any(pink)}}});

	spaghetti = AssetManager::GetInstance()->LoadSkybox("assets/game.cubedef.yml").get();

	Logging::ClientLog("Generating world...");

	World& world = WorldManager::GetInstance()->GetWorld("Game");
	world.skybox = spaghetti;

	int blkc = 1;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 8);
	for(int x = 0; x < 10; x++) {
		for(int z = 0; z < 10; z++) {
			for(int y = 0; y < 20; y++) {
				std::shared_ptr<Entity> blk = std::make_shared<Entity>(std::to_string(blkc));
				blk->GetLocalTransform().SetPosition({float(x), float(y - 10), -float(z + 4)});
				blk->GetLocalTransform().SetScale({1.0f, 1.0f, 1.0f});
				blk->SetActive(true);
				blk->IsActive();
				std::shared_ptr<MeshComponent> mc = blk->GetComponent<MeshComponent>(blk->MountComponent<MeshComponent>());
				mc->mesh = block;
				switch(dist(rng)) {
					case 0:
						mc->mat = redM;
						break;
					case 1:
						mc->mat = orangeM;
						break;
					case 2:
						mc->mat = goldM;
						break;
					case 3:
						mc->mat = greenM;
						break;
					case 4:
						mc->mat = cyanM;
						break;
					case 5:
						mc->mat = lblueM;
						break;
					case 6:
						mc->mat = dblueM;
						break;
					case 7:
						mc->mat = purpleM;
						break;
					case 8:
						mc->mat = pinkM;
						break;
				}
				mc->SetActive(true);
				blk->SetParent(world.rootEntity);

				blkc++;
				blks.push_back(blk);
			}
		}
	}

	camMgrEnt = std::make_shared<Entity>("Cam Manager");
	cmGUID = camMgrEnt->MountComponent<CamMgr>();
	camMgrEnt->SetParent(world.rootEntity);
	camMgrEnt->SetActive(true);
	camMgrEnt->GetComponent<CamMgr>(cmGUID)->SetActive(true);
	camMgrEnt->GetComponent<ExitHandler>(camMgrEnt->MountComponent<ExitHandler>())->SetActive(true);

	Logging::ClientLog("Thretris is started.");
}

void Thretris::OnShutdown() {
	Logging::ClientLog("Thretris is stopped.");
}
