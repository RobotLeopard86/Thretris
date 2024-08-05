#pragma once

#include "Cacao.hpp"

class CamMgr final : public Cacao::Script {
  public:
	void OnActivate() override {}
	void OnDeactivate() override {}
	void OnTick(double timestep) override {
		Cacao::World& world = Cacao::WorldManager::GetInstance()->GetActiveWorld();
		Cacao::PerspectiveCamera* cam = static_cast<Cacao::PerspectiveCamera*>(world.cam);
		glm::vec3 camRotChange = glm::vec3(0.0f);
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_J)) {
			camRotChange.y -= 0.5f;
		}
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_K)) {
			camRotChange.y += 0.5f;
		}
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_Y)) {
			camRotChange.x += 0.5f;
		}
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_U)) {
			camRotChange.x -= 0.5f;
		}

		currentRot = cam->GetRotation();
		currentRot += camRotChange;

		if(currentRot.x < -89.99) {
			currentRot.x = -89.99f;
		}
		if(currentRot.x > 89.99) {
			currentRot.x = 89.99f;
		}
		if(currentRot.y < 0) {
			currentRot.y = 360.0f;
		}
		if(currentRot.y > 360) {
			currentRot.y = 0.0f;
		}
		if(currentRot.z < 0) {
			currentRot.z = 360.0f;
		}
		if(currentRot.z > 360) {
			currentRot.z = 0.0f;
		}

		glm::vec3 posChange = glm::vec3(0.0f);
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_W)) {
			posChange += cam->GetFrontVector() * 5.0f * float(timestep);
		}
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_S)) {
			posChange -= cam->GetFrontVector() * 5.0f * float(timestep);
		}
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_D)) {
			posChange += cam->GetRightVector() * 5.0f * float(timestep);
		}
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_A)) {
			posChange -= cam->GetRightVector() * 5.0f * float(timestep);
		}
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_E)) {
			posChange += cam->GetUpVector() * 5.0f * float(timestep);
		}
		if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_Q)) {
			posChange -= cam->GetUpVector() * 5.0f * float(timestep);
		}

		currentPos = cam->GetPosition() + posChange;

		cam->SetRotation(currentRot);
		cam->SetPosition(currentPos);
	}

  private:
	glm::vec3 currentRot, currentPos;
};