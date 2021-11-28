#include "CameraController.h"

//Controller to follow the pllayer
CameraController::CameraController(Player* target, Camera* camera)
{
	this->target = target;
	this->camera = camera;
}

void CameraController::followTarget()
{
	camera->SetPosition(target->boat->position.x, target->boat->position.y, camera->m_Position.z);
}