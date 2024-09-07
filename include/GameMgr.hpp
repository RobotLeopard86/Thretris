#pragma once

#include "Cacao.hpp"

using namespace Cacao;

#include "Thretromino.hpp"

using std::chrono::steady_clock;

constexpr std::chrono::milliseconds prespawnTime = std::chrono::milliseconds(1000);

enum class State {
	Move,
	Spawn,
	UsrIn,
	CheckClear,
	GameOver
};

struct MoveStateArgs {
	glm::vec3 tgt, original, dir;
	float curDist, totDist;
};

class GameMgr final : public Script {
  public:
	void OnActivate() override {
		dropFinished = steady_clock::now() - prespawnTime;
		state = State::Spawn;
		numSpawns = 0;
	}
	void OnTick(double) override;

  private:
	std::shared_ptr<Thretromino> activeThretro;
	steady_clock::time_point dropFinished;
	int numSpawns;
	State state;
	MoveStateArgs msa;
};