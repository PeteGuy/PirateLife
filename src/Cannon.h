#pragma once
#include "Macros.h"
#include "RigidBody.h"
#include "3d/model.h"
class Cannon
{
public:
	Cannon(glm::vec2 baseDir,float fov,RigidBody* parent = NULL,glm::vec2 relativePos = glm::vec2());
	void update();
	void render(float zPosition = 0);
	void renderAimLine(float zPosition = 0);
	void aimTowardsTarget(glm::vec2 targetPos);
	void tryToFire(int layer);
	void renderProgressBar();
	

	glm::vec2 position;
	float angleToTarget;
	
private:
	
	glm::vec2 baseDirection;
	glm::vec2 direction = glm::vec2(0,0);
	float angle; //(degrees )
	float angularVelocity;
	float defaultAngularVelocity = 45;
	float fieldOfView;//(degrees )
	RigidBody* target = NULL;
	glm::vec2 targetPosition;

	//A cannon has to be attached to a parent body
	RigidBody* parent;
	glm::vec2 relativePosition;
	bool canFire;
	float coolDown;
	float coolDownTime = 3.0f;
	Model* model;
	float fireSpeed = 10.0f;



};