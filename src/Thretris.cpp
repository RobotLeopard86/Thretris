#include "Thretris.hpp"

#include <cstdlib>
#include <cstddef>

#include "CamMgr.hpp"

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
	startText->SetSize({0.4f, 0.1f});
	startText->SetText("Press A to start");
	startText->SetAlignment(TextAlign::Left);
	startText->SetColor({255.0f, 255.0f, 255.0f});
	startText->SetOffsetFromAnchor({-0.0375f, -0.1f});
	font = fontFut.get();
	startText->SetFont(font);
	startText->SetDepth(0);
	startText->SetActive(true);
	mainMenu->AddElement(startText);
	Engine::GetInstance()->GetGlobalUIView()->SetScreen(mainMenu);

	static EventConsumer* aPressAddr;
	EventConsumer* onAPress = new EventConsumer([this](Event& e) {
		Engine::GetInstance()->GetThreadPool()->enqueue_detach([this]() {
			Logging::ClientLog("Loading assets...");

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
			texLoadOp.WaitAll();
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

			Logging::ClientLog("Prepping world...");

			World& world = WorldManager::GetInstance()->GetWorld("Game");
			world.skybox = spaghetti;
			int blkc = 1;
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
						mc->mat = dblueM;
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

			Logging::ClientLog("Let's go!");

			gameUI = std::make_shared<Screen>();
			WorldManager::GetInstance()->SetActiveWorld("Game");
			Engine::GetInstance()->GetGlobalUIView()->SetScreen(gameUI);

			if(aPressAddr) {
				EventManager::GetInstance()->UnsubscribeConsumer("KeyDown", aPressAddr);
				delete aPressAddr;
			}
		});
	});
	aPressAddr = onAPress;
	EventManager::GetInstance()->SubscribeConsumer("KeyDown", onAPress);

	Logging::ClientLog("Thretris is started.");
}

void Thretris::OnShutdown() {
	Logging::ClientLog("Thretris is stopped.");
}
