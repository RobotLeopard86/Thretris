#pragma once

#include "Cacao.hpp"

class CamMgr final : public Cacao::Script {
  public:
	void OnActivate() override {
		auto cam = Cacao::WorldManager::GetInstance()->GetActiveWorld().cam;
		currentPos = cam->GetPosition();
		currentRot = cam->GetRotation();
		orbit = glm::vec3 {0};
	}
	void OnDeactivate() override {}
	void OnTick(double timestep) override;

  private:
	glm::vec3 currentRot, currentPos;
	glm::vec3 orbit;
};