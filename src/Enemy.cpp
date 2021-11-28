#include "Enemy.h"
#include "Render.h"
#include "ModelManager.h"
#include "GameProcess.h"
#include "timeOperations.h"
#include "Debug.h"

BaseEnemy::BaseEnemy(RigidBody& b)//Base enemy, does not move
{
	body = std::make_unique<RigidBody>(b);
	maxHealth = 20;
	health = maxHealth;
	active = false;
	model = ModelManager::basicEnemy.get();
	aimTimer = 0;
	fireTimer = 0;
	playerPointer = GameProcess::player.get();

	detectionRadius = 5;
	cannon = std::make_unique<Cannon>(glm::vec2(1, 0), 90, body.get(), glm::vec2(0.25f,0));
	layer = 2;
	
}

void Enemy::renderLifeBar()
{
	Render::renderProgressBar(health / maxHealth, body->position + glm::vec2(0,1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 2);
}

void BaseEnemy::takeDamage(float damage)
{
	health -= damage;
	
}


void BaseEnemy::update()
{
	body->update();
	body->applyMovementFriction();
	cannon->update();

	//If the player is close enough aim at him and shoot periodically
	if (glm::length2(playerPointer->boat->position - body->position) < detectionRadius * detectionRadius)
	{
		
		aimTimer -= dtValue;
		if (aimTimer <= 0)
		{
			
			currentTargetPosition = playerPointer->boat->position;
			aimTimer = baseAimTimer;
		}
		cannon->aimTowardsTarget(currentTargetPosition);
		fireTimer -= dtValue;
		if (fireTimer <= 0)
		{
			cannon->tryToFire(layer);
			fireTimer = baseFireTimer;
		}

	}
	else
	{
		aimTimer = baseAimTimer;
		fireTimer = baseFireTimer;
	}

	if (health <= 0)
	{
		die();
	}

	
}
void BaseEnemy::render()
{
	//Render::renderBody(body.get());

	float zPosition = Render::seaHeight(body->position);
	Render::renderModel(model, body->position, body->angle, zPosition);

	cannon->render(zPosition);
	Render::renderProgressBar(health / maxHealth, body->position + glm::vec2(0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 2,false,zPosition+0.5f);

}


void BaseEnemy::die()
{
	std::cout << "this enemy has perished\n";
	active = false;
	health = maxHealth;
	GameProcess::enemyCount--;
	GameProcess::treasures.push_back(std::make_unique<Treasure>(body->position));
}

//Obstacle, not currently used in the game 
ObstacleEnemy::ObstacleEnemy(RigidBody& b)
{
	body = std::make_unique<RigidBody>(b);
	body->mass = 0;
	body->inv_mass = PhysicData::calculateSafeInverse(body->mass);
	model = ModelManager::obstacle.get();
	active = true;
	layer = 3;
}
void ObstacleEnemy::takeDamage(float damage)
{

}
void ObstacleEnemy::update()
{
	body->update();
	
}
void ObstacleEnemy::render()
{
	//draw the model
	//Render::renderBody(body.get());

	glm::vec3 lightPos = glm::vec3(0, 0, 1000);
	glm::vec3 lightColor = glm::vec3(1, 1, 1);
	glm::mat4 modelMat4 = glm::mat4(1);
	modelMat4 = glm::translate(modelMat4, glm::vec3(0, 0, 0));
	modelMat4 = glm::translate(modelMat4, glm::vec3(body->position, 0));
	modelMat4 = glm::rotate(modelMat4, glm::radians(90.0f), glm::vec3(1, 0, 0));
	modelMat4 = glm::rotate(modelMat4, body->angle, glm::vec3(0, 1, 0));
	modelMat4 = glm::scale(modelMat4, glm::vec3(1, 1, 1));
	model->Draw(Render::defaultCamera, modelMat4, lightPos, lightColor);
	

	
}

void ObstacleEnemy::die()
{

}

//Mobile enemy, alternates between states in order to look for the player, pursue and attack
MobileEnemy::MobileEnemy(RigidBody& b)
{
	body = std::make_unique<RigidBody>(b);
	maxHealth = 60;
	health = maxHealth;
	active = false;
	model = ModelManager::boat.get();//Pour l'instant on utilise le modèle joueur
	aimTimer = 0;
	fireTimer = 0;
	playerPointer = GameProcess::player.get();

	detectionRadius = 10;
	cannon = std::make_unique<Cannon>(glm::vec2(1, 0), 90, body.get(), glm::vec2(0.25f, 0));
	layer = 2;
	body->angularVelocity = 0.5f;
	body->mass = 2000.0f;
	body->inv_mass = PhysicData::calculateSafeInverse(2000.0f);
	state = EnemyState::patrolling;
}

void MobileEnemy::takeDamage(float damage) 
{
	health -= damage;
}

void MobileEnemy::update()
{
	float myPi = glm::pi<float>();
	float dotProduct;
	float angleOfTarget = 0;
	float  bodyAngle = 0;
	glm::vec2 vectorToTarget;
	body->update();
	cannon->update();
	body->applyRotationFriction();
	assert(playerPointer != nullptr);
	float distanceToPlayer = glm::length2(playerPointer->boat->position - body->position);
	switch (state)
	{
	case EnemyState::patrolling: //During patrol the enemy simply checks his distance to the player
		aimTimer = 0;
		fireTimer = 0;
		if (distanceToPlayer<20*20)
		{
			state = EnemyState::pursuing;
		}
		break;
	case EnemyState::pursuing://During pursuit, the enemy attempts to get close enough to the player to fire at him
		aimTimer = 0;
		fireTimer = 0;
		currentTargetPosition = playerPointer->boat->position;
		

		vectorToTarget = currentTargetPosition - body->position;



		angleOfTarget = atan2(vectorToTarget.y, vectorToTarget.x);

		bodyAngle = body->angle;
		bodyAngle += myPi / 2 - angleOfTarget;
		if (bodyAngle > 0)
		{
			while (bodyAngle > 2 * myPi)
			{
				bodyAngle -= 2 * myPi;
			}
		}
		while (bodyAngle < 0)
		{

			bodyAngle += 2 * myPi;
		}



		if (bodyAngle < myPi)
		{
			body->angularVelocity = -1.5f;
		}
		else if (bodyAngle > myPi)
		{
			body->angularVelocity = 1.5f;
		}






		dotProduct = glm::dot(body->rotationMat * glm::vec2(0, 1), vectorToTarget);
		if (dotProduct > 0)
		{
			if (glm::length2(body->velocity) != 0)
			{
				float dot = glm::dot(body->rotationMat * glm::vec2(0, 1), glm::normalize(body->velocity));
				float factor = -0.5 * dot + 0.5;
				if (dot < 0)
				{
					body->velocity *= 0.25f;
				}

				body->applyForce( 5.0f * body->mass * body->rotationMat * glm::vec2(0, 1));
			}
			else
			{
				body->applyForce(10.0f * body->rotationMat * glm::vec2(0, 1));
			}
		}
		else
		{
			body->applyMovementFriction();
		}
		
		if (distanceToPlayer > 20 * 20)
		{
			state = EnemyState::patrolling;
		}
		else if (distanceToPlayer < 10 * 10)
		{
			state = EnemyState::attacking;
		}
		break;
	case EnemyState::attacking:// During an attack, the boat turns so that his cannon is pointed in the general direction of the player and the, aims and fires periodically like a base enemy
		body->applyMovementFriction();
		

			aimTimer -= dtValue;
			currentTargetPosition = playerPointer->boat->position;

			cannon->aimTowardsTarget(currentTargetPosition);
			fireTimer -= dtValue;
			if (fireTimer <= 0)
			{
				cannon->tryToFire(layer);
				fireTimer = baseFireTimer;
			}

		
		
		vectorToTarget = currentTargetPosition - body->position;




		angleOfTarget = atan2(vectorToTarget.y, vectorToTarget.x);

		bodyAngle = body->angle;

		bodyAngle +=  -angleOfTarget;
		if (bodyAngle > 0)
		{
			while (bodyAngle > 2 * myPi)
			{
				bodyAngle -= 2 * myPi;
			}
		}
		while (bodyAngle < 0)
		{

			bodyAngle += 2 * myPi;
		}



		if (bodyAngle < myPi)
		{
			body->angularVelocity = -1.5f;
		}
		else if (bodyAngle > myPi)
		{
			body->angularVelocity = 1.5f;
		}
		if (bodyAngle < 0.05f  || bodyAngle>2*myPi - 0.05f)
		{
			body->angularVelocity = 0;
		}

		

		if (distanceToPlayer > 20 * 20)
		{
			state = EnemyState::patrolling;
		}
		else if (distanceToPlayer > 10 * 10)
		{
			state = EnemyState::pursuing;
		}
		break;
	}
	
	if (health <= 0)
	{
		die();
	}

	
		

	
}

void MobileEnemy::render()
{
	float zPosition = Render::seaHeight(body->position);
	Render::renderModel(model, body->position, body->angle, zPosition);

	cannon->render(zPosition);
	Render::renderProgressBar(health / maxHealth, body->position + glm::vec2(0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 2, false, zPosition + 0.5f);
}

void MobileEnemy::die()
{
	std::cout << "this enemy has perished\n";
	active = false;
	health = maxHealth;
	GameProcess::enemyCount--;
	GameProcess::treasures.push_back(std::make_unique<Treasure>(body->position));
}