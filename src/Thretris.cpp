#include "Thretris.hpp"

#include <cstdlib>

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
	Logging::ClientLog("Thretris is up and running!");
}

void Thretris::OnShutdown() {
	Logging::ClientLog("Thretris is stopped.");
}
