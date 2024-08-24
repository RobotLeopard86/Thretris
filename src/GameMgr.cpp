#include "GameMgr.hpp"

#include "Thretris.hpp"

void GameMgr::OnTick(double timestep) {
	decltype(Thretris::GetInstance()->blks) blks = Thretris::GetInstance()->blks;
	switch(state) {
		case State::Spawn: {
			if(steady_clock::now() - dropFinished < prespawnTime) break;
			auto rng = std::mt19937(std::random_device()());
			activeThretro = SpawnThretromino((ThretrominoType)std::uniform_int_distribution<int>(0, 8)(rng));
			activeThretro->center = {4, 19, 5};
			activeThretro->UpdateInWorld();
			state = State::UsrIn;
			break;
		}
		case State::UsrIn: {
			msa.tgt = activeThretro->center;
			msa.tgt.y = ceil(msa.tgt.y) - 1;
			if(msa.tgt.y < 0) goto freeze;

			glm::vec3 noMovementTgt = msa.tgt;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_UP)) msa.tgt.x++;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_DOWN)) msa.tgt.x--;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_LEFT)) msa.tgt.z--;
			if(Input::GetInstance()->IsKeyPressed(CACAO_KEY_RIGHT)) msa.tgt.z++;

			uint8_t iShift = 0;
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

			bool moveIsValid = true;
			for(glm::i8vec3 member : activeThretro->shapes[activeThretro->idx]) {
				glm::vec3 p = glm::vec3 {round(member.x), ceil(member.y), round(member.z)} + msa.tgt;
				if(p.x < 0 || p.x > 9 || p.z < 0 || p.z > 9 || p.y < 0 || p.y > 19) {
					moveIsValid = false;
					break;
				} else if(blks[p.x][9 - p.z][p.y]) {
					moveIsValid = false;
					break;
				}
			}

			if(!moveIsValid) {
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
					bool ok = true;
					for(glm::i8vec3 member : activeThretro->shapes[activeThretro->idx]) {
						glm::vec3 p = glm::vec3 {round(member.x), ceil(member.y), round(member.z)} + tgt;
						if(p.x < 0 || p.x > 9 || p.z < 0 || p.z > 9 || p.y < 0 || p.y > 19) {
							ok = false;
							break;
						} else if(blks[p.x][9 - p.z][p.y]) {
							ok = false;
							break;
						}
					}
					if(!ok) continue;
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
			msa.curDist += (float(timestep)) * 3;
			activeThretro->center = (msa.curDist >= msa.totDist ? msa.tgt : msa.original + (msa.dir * (msa.curDist / msa.totDist)));
			activeThretro->UpdateInWorld();
			if(activeThretro->center == msa.tgt) {
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
					for(uint8_t i = 0; i < 10; i++) {
						blks[r][i][y]->SetParent(blks[r][i][y]);
						blks[r][i][y] = std::shared_ptr<Entity>();
						cleared.emplace_back(r, i, y);
					}
				}
				for(uint8_t c : columnsToClear) {
					for(uint8_t i = 0; i < 10; i++) {
						if(!blks[i][c][y]) continue;
						blks[i][c][y]->SetParent(blks[i][c][y]);
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
							blks[x][z][y].swap(blks[x][z][y]);
						}
					}
				}
			}

			Thretris::GetInstance()->blks = blks;

			state = State::UsrIn;
			break;
		}
	}
	return;

freeze:
	activeThretro->Freeze();
	dropFinished = steady_clock::now();
	state = State::Spawn;
	return;
}