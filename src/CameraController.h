#pragma once
#include "Macros.h"
#include "3d/camera.h"
#include "Player.h"


//Simple camera controller class to follow the player
class CameraController
{
public:
	CameraController(Player* target, Camera* camera);
	void followTarget();
private:
	Player* target;
	Camera* camera;
};