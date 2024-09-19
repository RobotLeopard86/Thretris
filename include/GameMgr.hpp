#pragma once

#include "Cacao.hpp"

using namespace Cacao;

#include "Thretromino.hpp"

using std::chrono::steady_clock;

constexpr std::chrono::milliseconds prespawnTime = std::chrono::milliseconds(1000);
constexpr std::chrono::milliseconds pauseCooldown = std::chrono::milliseconds(2000);
constexpr std::chrono::milliseconds unpauseCooldown = std::chrono::milliseconds(1000);

enum class State {
	Move,
	Spawn,
	UsrIn,
	CheckClear,
	GameOver,
	Pause
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
		lastUnpause = steady_clock::now();
		lastPause = steady_clock::now();
	}
	void OnTick(double) override;

	static void SignalGameOver();

  private:
	std::shared_ptr<Thretromino> activeThretro;
	steady_clock::time_point dropFinished;
	int numSpawns;
	State state;
	MoveStateArgs msa;

	steady_clock::time_point lastUnpause;
	steady_clock::time_point lastPause;

	static bool gameOverSig;
};