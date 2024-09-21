#include "CamMgr.hpp"

#include "Thretris.hpp"

constexpr glm::vec3 stackCenter = {10, -2, 0};

#define SPEED 6.5f
#define RADIUS 24.0f

void CamMgr::OnTick(double timestep) {
	if(Thretris::GetInstance()->IsPaused()) return;

	if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_W)) {
		orbit.x += SPEED * timestep;
	}
	if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_A)) {
		orbit.y += SPEED * timestep;
	}
	if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_S)) {
		orbit.x -= SPEED * timestep;
	}
	if(Cacao::Input::GetInstance()->IsKeyPressed(CACAO_KEY_D)) {
		orbit.y -= SPEED * timestep;
	}

	if(orbit.y >= 360) orbit.y -= 360;
	if(orbit.y < 0) orbit.y += 360;
	orbit.x = std::clamp(orbit.x, 0.0f, 89.9f);
	orbit.z = 0.0f;

	float tilt = glm::radians(orbit.x);
	float pan = glm::radians(orbit.y);
	currentPos.x = cos(tilt) * cos(pan);
	currentPos.y = sin(tilt);
	currentPos.z = cos(tilt) * sin(pan);
	currentPos = glm::normalize(currentPos);
	currentPos *= RADIUS;
	currentPos += stackCenter;

	currentRot = glm::vec3 {-orbit.x, 180 - (-orbit.y), 0.0f};
	if(currentRot.y >= 360) currentRot.y -= 360;
	if(currentRot.y < 0) currentRot.y += 360;

	Cacao::PerspectiveCamera* cam = static_cast<Cacao::PerspectiveCamera*>(Cacao::WorldManager::GetInstance()->GetActiveWorld().cam);
	cam->SetRotation(currentRot);
	cam->SetPosition(currentPos);
}