#include "Cannon.h"
#include "timeOperations.h"
#include "Debug.h"
#include "PoolManager.h"
#include "Render.h"
#include "ModelManager.h"


Cannon::Cannon(glm::vec2 baseDir, float fov, RigidBody* parent, glm::vec2 relativePos)
{
	baseDirection = baseDir;
	fieldOfView = fov;
	this->parent = parent;
	relativePosition = relativePos;
	coolDown = coolDownTime;
	canFire = true;
	model = ModelManager::cannon.get();
}

void Cannon::render(float zPosition)
{
	

	//The render differs based on the angle of the cannon
	if (baseDirection.x > 0)
	{

		Render::renderModel(model, position, glm::radians(angle) + parent->angle, zPosition+0.1f);
	}
	else
	{

		Render::renderModel(model, position, glm::radians(angle+180) + parent->angle, zPosition+0.1f);
	}
	
	

	//Render reload bar if reloading
	if (!canFire)
	{
		Render::renderProgressBar(1 - (coolDown / coolDownTime), position + parent->rotationMat * glm::vec2(1, 0), glm::vec3(1, 1, 0), glm::vec3(0.5f, 0, 0), 0.5f, true,zPosition+2);
	}
	
	
}

//Render aimLine that shows where the cannon is aiming
void Cannon::renderAimLine(float zPosition)
{
	Debug::drawDebugVector(position, direction * 2.0f,zPosition+0.1f);
}


void Cannon::update()
{
	if (!canFire)
	{
		coolDown -= dtValue;
	}
	if (coolDown <= 0)
	{
		canFire = true;
		coolDown = coolDownTime;
	}

	
	position = parent->position + parent->rotationMat * relativePosition;

	

	
	float myPi = glm::pi<float>();

	//Choose angular velocity depending on target position 
	if (angleToTarget > 0)
	{
		if (angleToTarget > myPi)
		{
			angularVelocity = -defaultAngularVelocity;
		}
		else
		{
			angularVelocity = defaultAngularVelocity;
		}
		
	}
	else if (angleToTarget < 0)
	{
		if (angleToTarget < -myPi)
		{
			angularVelocity = defaultAngularVelocity;
		}
		else
		{
			angularVelocity = -defaultAngularVelocity;
		}
		
	}
	else
	{
		angularVelocity = 0;
	}
	
	


	//Don't rotate if target is close enough to the aim line
	if (angleToTarget*angleToTarget < 0.01f)
	{
		angularVelocity = 0;
	}

	angle += angularVelocity*dtValue;
	angleToTarget-= angularVelocity * dtValue;

	

	if (angle > fieldOfView / 2)
	{
		angle = fieldOfView / 2;
	}
	else if (angle < -fieldOfView / 2)
	{
		angle = -fieldOfView / 2;
	}

	
	//Recalculate the direction of the cannon
	direction = parent->rotationMat * glm::mat2(cos(glm::radians(angle)), sin(glm::radians(angle)), -sin(glm::radians(angle)), cos(glm::radians(angle))) * baseDirection;

}


void Cannon::aimTowardsTarget(glm::vec2 targetPos)
{

	targetPosition = targetPos;
	float myPi = glm::pi<float>();
	
	glm::vec2 vectorToTarget = targetPosition - position;
	glm::vec2 parentToTarget = targetPosition - parent->position;


	parentToTarget = parent->rotationMat * parentToTarget;

	//Get angle from canon direction to target
	float angleOfTarget = atan2(vectorToTarget.y, vectorToTarget.x);
	
	float angleParentTarget = atan2(parentToTarget.y, parentToTarget.x);
	float parentAngle = parent->angle;
	if (parentAngle > 0)
	{
		while (parentAngle > 2 * myPi)
		{
			parentAngle -= 2 * myPi;
		}
	}
	if (parentAngle < 0)
	{
		while (parentAngle < -2 * myPi)
		{
			parentAngle += 2 * myPi;
		}
	}
	float angleOfCanon = atan2(direction.y, direction.x);

	


	

	if (glm::dot(direction, vectorToTarget) < 0)
	{
		angleToTarget = 0;
	}
	else
	{
		angleToTarget = angleOfTarget - angleOfCanon;
	}
	

	

	

	
}


void Cannon::tryToFire(int layer)
{
	if (canFire)
	{
		PoolManager::getInstance()->launchAvailableCannonBall(position + direction*0.5f, direction, fireSpeed,layer);
		canFire = false;
	}
}

void Cannon::renderProgressBar()
{
	if (!canFire)
	{
		Render::renderProgressBar(1 - (coolDown / coolDownTime), position+ parent->rotationMat*glm::vec2(1, 0), glm::vec3(1, 1, 0), glm::vec3(0.5f, 0,0), 0.5f, true);
	}
	
}
