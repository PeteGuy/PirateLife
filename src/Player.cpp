#include "Player.h"
#include "Render.h"
#include "Debug.h"
#include "timeOperations.h"
#include "ModelManager.h"



Player::Player(RigidBody *body, InputManager iM)
{
	boat = body;
	inputManager = iM;
	impulsePoint = glm::vec2(0, body->shape->vecPoints[0].y);
	direction = glm::vec2(0, 1);
	angle = 0;
	angleInputSpeed = 3;
	windForce = 0;
	cannonCount = 0;
	currentCannonIndex = 0;
	layer = 1;
	addCannon();
	boat->mass = 2000.0f;
	boat->inv_mass = PhysicData::calculateSafeInverse(2000.0f);
	
	model = ModelManager::boat.get();
	health = maxHealth;
}


void Player::render()
{
	float zPosition = Render::seaHeight(boat->position);
	float xAngle = Render::xSeaAngle(boat->position);
	float yAngle = Render::ySeaAngle(boat->position);

	Render::renderModel(model, boat->position, boat->angle,zPosition,xAngle,yAngle);

	
	//Render::renderBody(boat);
	for (auto it = cannons.begin(); it != cannons.end(); it++)
	{
		(*it)->render(zPosition);
	
	}
	cannons[currentCannonIndex]->renderAimLine(zPosition);

	Render::renderProgressBar(health / maxHealth, boat->position + glm::vec2(-1, -2), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 2,false,zPosition+0.2f);
}



void Player::addToAngle(int factor)
{
	
	float quarterPi = glm::quarter_pi<float>();
	if (angle > quarterPi)
	{
		angle = quarterPi;
	}
	if (angle < -quarterPi)
	{
		angle = -quarterPi;
	}
	

	calculateDirection();
}
void Player::calculateDirection()
{
	
}


void Player::reset()
{
	boat->velocity.x = 0;
	boat->velocity.y = 0;
	boat->position.x = 0;
	boat->position.y = 0;
	boat->angularVelocity = 0;
	boat->angle = 0;
	boat->rotationMat = glm::mat2(1);
}

void Player::reset(float x, float y)
{
	reset();
	boat->position.x = x;
	boat->position.y = y;
}


void Player::update()
{
	if (inputManager.isKeyPressed(inputManager.switchCannonLeft))
	{
		if (!previouslySwitchingCannon)
		{
			switchCurrentCannon(false);
			previouslySwitchingCannon = true;
		}
		
	}
	else if (inputManager.isKeyPressed(inputManager.switchCannonRight))
	{
		
		if (!previouslySwitchingCannon)
		{
			switchCurrentCannon(true);
			previouslySwitchingCannon = true;
		}
	}
	else
	{
		previouslySwitchingCannon = false;
	}
	
	if (inputManager.isKeyPressed(inputManager.pushForwardKey))
	{
		if (glm::length2(boat->velocity) != 0)
		{
			float dot = glm::dot(boat->rotationMat * glm::vec2(0, 1),glm::normalize(boat->velocity));
			float factor = -0.5 * dot + 0.5;
			//Debug::
			("factor", factor);
			boat->applyForce(3.0f * boat->mass * boat->rotationMat * glm::vec2(0, 1));
			boat->applyForce(factor *3.0f * boat->mass * boat->rotationMat * glm::vec2(0, 1));
		}
		else
		{
			boat->applyForce(20.0f * boat->rotationMat * glm::vec2(0, 1));
		}
		
		
	}
	else
	{
		boat->applyMovementFriction();
	}
	if (inputManager.isKeyPressed(inputManager.leftSideImpulse))
	{
		
		//addToAngle(-1);
		if (boat->angularVelocity > 0)
		{
			boat->angularVelocity += angleInputSpeed * dtValue;
		}
		else
		{
			boat->angularVelocity += 2*angleInputSpeed * dtValue;
		}
		
	}
	else if (inputManager.isKeyPressed(inputManager.rightSideImpulse))
	{
		//addToAngle(1);
		if (boat->angularVelocity < 0)
		{
			boat->angularVelocity += -angleInputSpeed * dtValue;
		}
		else
		{
			boat->angularVelocity += 2*(-angleInputSpeed) * dtValue;
		}
		
	}
	else
	{
		boat->applyRotationFriction();
	}

	boat->update();
	for (auto it = cannons.begin(); it != cannons.end(); it++)
	{
		(*it)->update();
		(*it)->aimTowardsTarget(Render::ScreenToWorldCoordinates(inputManager.getTargetLoc()));
		
		
	}

	if (inputManager.isKeyPressed(inputManager.fireKey))
	{
		cannons[currentCannonIndex]->tryToFire(layer);
	}

}



void Player::addCannon()
{
	if (cannonCount < 4)
	{
		if (cannonCount < 2)
		{
			
			cannons.push_back(std::make_unique<Cannon>(glm::vec2(1, 0), 90, boat, glm::vec2(0.25f, -0.4f+ 0.5f*(cannonCount))));
			cannonCount++;
		}
		else
		{
			
			cannons.push_back(std::make_unique<Cannon>(glm::vec2(-1, 0), 90, boat, glm::vec2(-0.25f, -0.4f + 0.5f * (cannonCount-2))));
			cannonCount++;
		}
		
	}
	
}
void Player::switchCurrentCannon(bool sens)
{
	if (sens)
	{
		currentCannonIndex++;
		
		
	}
	else
	{
		currentCannonIndex--;
	}

	if (currentCannonIndex > cannonCount - 1)
	{
		currentCannonIndex = 0;
		//currentCannon
	}
	else if (currentCannonIndex < 0)
	{
		currentCannonIndex = cannonCount - 1;
	}
	

}

void Player::renderLifeBar()
{
	Render::renderProgressBar(health / maxHealth, boat->position + glm::vec2(-1, -2), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 2);
}

void Player::takeDamage(float damage)
{
	health -= damage;
	if (health <= 0)
	{
		hasLost = true;
	}
}