#pragma once
#include "Macros.h"
#include "RigidBody.h"
#include "InputManager.h"
#include "Cannon.h"
#include "3d/model.h"

class Player
{
public:
	Player(RigidBody *body,InputManager iM);
	void update();

	void render();
	void addToAngle(int factor);
	void calculateDirection();
	void reset();
	void reset(float x,float y);
	void renderLifeBar();
	void takeDamage(float damage);
	void addCannon();


	RigidBody* boat;
	int layer;
	bool hasLost = false;

private:
	
	glm::vec2 direction;
	glm::vec2 impulsePoint;
	InputManager inputManager;
	float angle;
	float angleInputSpeed;
	float windForce;
	glm::mat2 angleMat = glm::mat2();
	std::vector<std::unique_ptr<Cannon>> cannons;
	
	void switchCurrentCannon(bool sens);
	int currentCannonIndex;
	int cannonCount;
	Model* model;
	bool previouslySwitchingCannon = false;
	float health;
	float maxHealth = 100;

	
};