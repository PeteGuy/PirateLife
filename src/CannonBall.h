#pragma once
#include "Macros.h"
#include "RigidBody.h"
#include "3d/model.h"


class CannonBall
{
public:
	CannonBall (RigidBody* b);
	void launch(glm::vec2 startingPos, glm::vec2 dir, float speed);
	void update();
	void render();

	bool active;
	int layer;
	RigidBody* body;
	float lifeSpan;
	float defaultLifeSpan;

private:
	glm::vec2 direction;
	glm::vec2 startingPosition;
	float speed;
	Model* model;


};