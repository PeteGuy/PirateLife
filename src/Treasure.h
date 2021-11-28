#pragma once
#include "Macros.h"
#include "RigidBody.h"
#include "3d/model.h"

class Treasure
{
public:
	Treasure(glm::vec2 position);
	void render();
	void update();

	RigidBody body;

private:
	Model* model;

};