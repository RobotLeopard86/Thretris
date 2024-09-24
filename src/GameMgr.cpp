#include "GameMgr.hpp"

#include "Thretris.hpp"

#define QUICKDROP_MULTIPLIER 12
#define BASE_LEVELADVANCE 10

bool GameMgr::gameOverSig = false;

void GameMgr::SignalGameOver() {
	gameOverSig = true;
}

bool IsMoveValid(std::shared_ptr<Thretromino> thr, glm::vec3 tgt, decltype(Thretris::GetInstance()->blks)& blks) {
	for(glm::i8vec3 member : thr->shapes[thr->idx]) {
		glm::vec3 p = glm::vec3 {round(member.x), ceil(member.y), round(member.z)} + tgt;
		if(p.x < 0 || p.x > 9 || p.z < 0 || p.z > 9 || p.y < 0 || p.y > 19) {
			return false;
		} else if(blks[p.x][9 - p.z][p.y]) {
			return false;
		}
	}
	return true;
}

void GameMgr::OnTick(double timestep) {
	decltype(Thretris::GetInstance()->blks) blks = Thretris::GetInstance()->blks;
	if(gameOverSig) {
		state = State::GameOver;
		for(auto blk : activeThretro->blocks) {
			blk.first->SetParent(blk.first);
		}
		Thretris::GetInstance()->GameOver();
		gameOverSig = false;
		return;
	}
	switch(state) {
		case State::Spawn: {
			if(steady_clock::now() - dropFinished < prespawnTime) break;
			activeThretro = SpawnThretromino(next);
			Regen();
			activeThretro->center = {4, 19, 5};
			activeThretro->UpdateInWorld();
			state = State::UsrIn;
			numSpawns++;

			if(numSpawns >= ceil(BASE_LEVELADVANCE * (1.0f + ((Thretris::GetInstance()->GetLvl() - 1) / 10)))) Thretris::GetInstance()->SetLvl(Thretris::GetInstance()->GetLvl() + 1);

			break;
		}
		case State::UsrIn: {
			if(Input::GetInstance()->IsMouseButtonPressed(CACAO_MOUSE_BUTTON_MIDDLE)) {
				SignalGameOver();
				return;
			}

			if(steady_clock::now() >= (lastUnpause + pauseCooldown) && Input::GetInstance()->IsKeyPressed(CACAO_KEY_ENTER)) {
				Thretris::GetInstance()->ShowPauseUI();
				Thretris::GetInstance()->musicMaker->TogglePause();
				lastPause = steady_clock::now();
				state = State::Pause;
				break;
			}

			for(int x = 0; x < 10; x++) {
				for(int z = 0; z < 10; z++) {
					if(blks[x][z][19]) {
						state = State::GameOver;
						for(auto blk : activeThretro->blocks) {
							blk.first->SetParent(blk.first);
						}
						Thretris::GetInstance()->GameOver();
						return;
					}
				}
			}

			if(activeThretro->quickDropped) {
				state = State::Move;
				goto mv;
			}

			msa.tgt = activeThretro->center;
			msa.tgt.y = ceil(msa.tgt.y) - 1;
			if(msa.tgt.y < 0) goto freeze;

			for(glm::i8vec3 member : activeThretro->shapes[activeThretro->idx]) {
				glm::vec3 p = glm::vec3 {round(member.x), ceil(member.y), round(member.z)} + msa.tgt;
				if(p.y >= 19 && !IsMoveValid(activeThretro, msa.tgt, blks)) {
					SignalGameOver();
					return;
				}
			}

			if(activeThretro->center.y <= 18 && Input::GetInstance()->IsKeyPressed(CACAO_KEY_H)) {
				activeThretro->quickDropped = true;
				glm::vec3 lowestTarget = activeThretro->center;
				while(lowestTarget.y > 0 && IsMoveValid(activeThretro, lowestTarget, blks)) {
					lowestTarget.y -= 1;
				}
				msa.original = activeThretro->center;
				msa.tgt = lowestTarget;
				msa.tgt.x = round(std::clamp(msa.tgt.x, 0.0f, 9.0f));
				msa.tgt.z = round(std::clamp(msa.tgt.z, 0.0f, 9.0f));
				msa.tgt.y = ceil(std::clamp(msa.tgt.y, 0.0f, 19.0f));

				if(!IsMoveValid(activeThretro, msa.tgt, blks)) msa.tgt.y++;
				msa.tgt.y = ceil(std::clamp(msa.tgt.y, 0.0f, 19.0f));

				if(msa.tgt.y > activeThretro->center.y) {
					Logging::ClientLog("what", LogLevel::Warn);
					Logging::ClientLog(std::to_string(activeThretro->center.y), LogLevel::Warn);
					Logging::ClientLog(std::to_string(msa.tgt.y), LogLevel::Warn);
				}

				msa.dir = msa.tgt - msa.original;
				msa.curDist = 0.0f;
				msa.totDist = glm::length(msa.dir);
				state = State::Move;
				goto mv;
			}

			glm::vec3 noMovementTgt = msa.tgt;

			glm::vec3 mvBuf {0};
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_UP)) mvBuf.x++;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_DOWN)) mvBuf.x--;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_LEFT)) mvBuf.z--;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_RIGHT)) mvBuf.z++;

			float crot = WorldManager::GetInstance()->GetActiveWorld().cam->GetRotation().y;
			int rotZone = (static_cast<int>((crot + 45) / 90) % 4);
			switch(rotZone) {
				case 0: break;
				case 1: {
					float swap = -mvBuf.z;
					mvBuf.z = mvBuf.x;
					mvBuf.x = swap;
					break;
				}
				case 2:
					mvBuf.x = -mvBuf.x;
					mvBuf.z = -mvBuf.z;
					break;
				case 3:
					mvBuf.x = -mvBuf.x;
					{
						float swap = mvBuf.z;
						mvBuf.z = mvBuf.x;
						mvBuf.x = swap;
					}
					break;
				default: break;
			}

			msa.tgt += mvBuf;

			int8_t iShift = 0;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_X)) iShift--;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_C)) iShift++;
			int newIdx = activeThretro->idx + iShift;
			if(newIdx >= activeThretro->shapes.size()) newIdx = 0;
			if(newIdx < 0) newIdx = activeThretro->shapes.size() - 1;

			bool rotateIsValid = true;
			for(glm::i8vec3 member : activeThretro->shapes[newIdx]) {
				glm::vec3 p = glm::vec3 {round(member.x), ceil(member.y), round(member.z)} + msa.tgt;
				if(p.x < 0 || p.x > 9 || p.z < 0 || p.z > 9 || p.y < 0 || p.y > 19) {
					rotateIsValid = false;
					break;
				} else if(blks[p.x][9 - p.z][p.y]) {
					rotateIsValid = false;
					break;
				}
			}
			if(rotateIsValid) activeThretro->idx = newIdx;

			msa.tgt.x = round(std::clamp(msa.tgt.x, 0.0f, 9.0f));
			msa.tgt.z = round(std::clamp(msa.tgt.z, 0.0f, 9.0f));
			msa.tgt.y = ceil(std::clamp(msa.tgt.y, 0.0f, 19.0f));

			if(!IsMoveValid(activeThretro, msa.tgt, blks)) {
				if(msa.tgt == noMovementTgt) goto freeze;
				std::vector<glm::vec3> possibleTgts;
				glm::vec3 oldTgt = msa.tgt;
				possibleTgts.push_back(noMovementTgt);
				if(msa.tgt.x != noMovementTgt.x) possibleTgts.emplace_back(noMovementTgt.x, msa.tgt.y, msa.tgt.z);
				if(msa.tgt.z != noMovementTgt.z) possibleTgts.emplace_back(msa.tgt.z, msa.tgt.y, noMovementTgt.z);
				for(auto tgt : possibleTgts) {
					tgt.x = round(std::clamp(tgt.x, 0.0f, 9.0f));
					tgt.z = round(std::clamp(tgt.z, 0.0f, 9.0f));
					tgt.y = ceil(std::clamp(tgt.y, 0.0f, 19.0f));
					if(!IsMoveValid(activeThretro, tgt, blks)) continue;
					msa.tgt = tgt;
					break;
				}
				if(msa.tgt == oldTgt) goto freeze;
			}

			msa.tgt.x = round(std::clamp(msa.tgt.x, 0.0f, 9.0f));
			msa.tgt.z = round(std::clamp(msa.tgt.z, 0.0f, 9.0f));
			msa.tgt.y = ceil(std::clamp(msa.tgt.y, 0.0f, 19.0f));

			if(msa.tgt.x != activeThretro->center.x || msa.tgt.z != activeThretro->center.z) {
				activeThretro->center = {msa.tgt.x, activeThretro->center.y, msa.tgt.z};
				activeThretro->UpdateInWorld();
			}

			msa.original = activeThretro->center;
			msa.dir = msa.tgt - msa.original;
			msa.curDist = 0.0f;
			msa.totDist = glm::length(msa.dir);
			state = State::Move;
			break;
		}
		case State::Move: {
		mv:
			msa.curDist += (float(timestep)) * (activeThretro->quickDropped ? QUICKDROP_MULTIPLIER : std::min(log(1 + Thretris::GetInstance()->GetLvl()) / log(1.75), 6.0));
			activeThretro->center = (msa.curDist >= msa.totDist ? msa.tgt : msa.original + (msa.dir * (msa.curDist / msa.totDist)));
			activeThretro->UpdateInWorld();
			if(activeThretro->center == msa.tgt) {
				if(activeThretro->quickDropped) goto freeze;
				state = State::CheckClear;
				break;
			}
		}
		case State::CheckClear: {
			std::vector<glm::u8vec3> cleared;
			for(uint8_t y = 0; y < 20; y++) {
				std::vector<uint8_t> rowsToClear;
				std::vector<uint8_t> columnsToClear;
				for(uint8_t x = 0; x < 10; x++) {
					bool exitLp = false;
					for(uint8_t z = 0; z < 10; z++) {
						if(!blks[x][z][y]) {
							exitLp = true;
							break;
						}
					}
					if(exitLp) continue;
					rowsToClear.emplace_back(x);
				}
				for(uint8_t z = 0; z < 10; z++) {
					bool exitLp = false;
					for(uint8_t x = 0; x < 10; x++) {
						if(!blks[x][z][y]) {
							exitLp = true;
							break;
						}
					}
					if(exitLp) continue;
					columnsToClear.emplace_back(z);
				}

				for(uint8_t r : rowsToClear) {
					Thretris::GetInstance()->IncrementScore();
					for(uint8_t i = 0; i < 10; i++) {
						blks[r][i][y]->SetParent(blks[r][i][y]);
						blks[r][i][y] = std::shared_ptr<Entity>();
						cleared.emplace_back(r, i, y);
					}
				}

				for(uint8_t c : columnsToClear) {
					Thretris::GetInstance()->IncrementScore();
					for(uint8_t i = 0; i < 10; i++) {
						if(!blks[i][c][y]) continue;
						blks[i][c][y]->SetParent(blks[i][c][y]);
						blks[i][c][y] = std::shared_ptr<Entity>();
						cleared.emplace_back(i, c, y);
					}
				}
			}
			for(uint8_t x = 0; x < 10; x++) {
				for(uint8_t z = 0; z < 10; z++) {
					std::vector<uint8_t> clearedInStack;
					for(auto c : cleared) {
						if(c.x == x && c.y == z) clearedInStack.emplace_back(c.z);
					}
					std::sort(clearedInStack.begin(), clearedInStack.end(), std::greater<uint8_t>());
					for(uint8_t clearPoint : clearedInStack) {
						for(uint8_t y = clearPoint; y <= 18; y++) {
							if(!blks[x][z][y + 1]) continue;
							glm::vec3 p = blks[x][z][y + 1]->GetLocalTransform().GetPosition();
							p.y -= 1;
							blks[x][z][y + 1]->GetLocalTransform().SetPosition(p);
							blks[x][z][y].swap(blks[x][z][y + 1]);
						}
					}
				}
			}

			Thretris::GetInstance()->blks = blks;

			state = State::UsrIn;
			break;
		}
		case State::GameOver: {
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_ENTER)) {
			restart_game:
				for(int x = 0; x < 10; x++) {
					for(int z = 0; z < 10; z++) {
						for(int y = 0; y < 20; y++) {
							if(!blks[x][z][y]) continue;
							blks[x][z][y]->SetParent(blks[x][z][y]);
							blks[x][z][y] = std::shared_ptr<Entity>();
						}
					}
				}
				Thretris::GetInstance()->blks = blks;
				Thretris::GetInstance()->SetLvl(1);
				Thretris::GetInstance()->ResetScore();
				dropFinished = steady_clock::now();
				state = State::Spawn;
				numSpawns = 0;
				World& world = WorldManager::GetInstance()->GetWorld("Game");
				world.cam->SetPosition({0, 0, 0});
				world.cam->SetRotation({0, 0, 0});
				Thretris::GetInstance()->ShowGameUI();
				gameOverSig = false;
				Thretris::GetInstance()->musicMaker->Play();
				lastUnpause = steady_clock::now();
				break;
			}
		}
		case State::Pause: {
			if(steady_clock::now() >= (lastPause + unpauseCooldown) && Input::GetInstance()->IsKeyPressed(CACAO_KEY_ENTER)) {
				Thretris::GetInstance()->ShowGameUI();
				Thretris::GetInstance()->musicMaker->TogglePause();
				lastUnpause = steady_clock::now();
				state = State::UsrIn;
				break;
			}
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_Y)) {
				for(auto blk : activeThretro->blocks) {
					blk.first->SetParent(blk.first);
				}
				Thretris::GetInstance()->musicMaker->Stop();
				goto restart_game;
			}
			if(steady_clock::now() >= (lastPauseToggle + unpauseCooldown) && Input::GetInstance()->IsKeyPressed(CACAO_KEY_A)) {
				Thretris::GetInstance()->ToggleHowToPlay();
				lastPauseToggle = steady_clock::now();
			}
		}
	}
	return;

freeze:
	activeThretro->Freeze();
	dropFinished = steady_clock::now();
	state = State::Spawn;
	return;
}

void GameMgr::Regen() {
	auto rng = std::mt19937(std::random_device()());
	next = (ThretrominoType)std::uniform_int_distribution<int>(0, 8)(rng);
	Thretris::GetInstance()->SetNextThretromino(next);
}
