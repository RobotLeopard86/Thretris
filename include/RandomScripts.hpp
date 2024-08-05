#pragma once

#include "Cacao.hpp"

#include "Thretris.hpp"

using namespace Cacao;

class ExitHandler final : public Script {
  public:
	void OnTick(double) override {
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_ESCAPE)) {
			Cacao::Engine::GetInstance()->Stop();
			return;
		}
	}
};

class GameStarter final : public Script {
  public:
	void OnActivate() override {
		haveStarted = false;
	}
	void OnTick(double) override {
		if(!haveStarted && Input::GetInstance()->IsKeyPressed(CACAO_KEY_ENTER)) {
			haveStarted = true;
			Thretris::GetInstance()->DoStart();
		}
	}

  private:
	bool haveStarted;
};

class Dirtifier final : public Script {
  public:
	void OnTick(double) override {
		Engine::GetInstance()->GetGlobalUIView()->GetScreen()->ForceDirty();
	}
};