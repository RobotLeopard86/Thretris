#pragma once

#include "Cacao.hpp"

using namespace Cacao;

class Thretris {
  public:
	static Thretris* GetInstance();

	void OnStartup();
	void OnShutdown();

  private:
	static Thretris* instance;
	static bool instanceExists;
};
