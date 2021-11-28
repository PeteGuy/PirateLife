#include "PoolManager.h"
#include "GameProcess.h"

PoolManager* PoolManager::instance = nullptr;

//We create a pool for every entity to avoid runtime instanciation
PoolManager::PoolManager()
{
	
	for (int i = 0; i < 100;i++)
	{
		
		cannonBalls.push_back(std::make_unique<CannonBall>(new RigidBody(new CircleShape(0.15f),0,0)));
	}

	for (int i = 0; i < 50; i++)
	{
		auto body = RigidBody(new CircleShape(0.5f), 0, 0);
		baseEnemies.push_back((std::make_unique<BaseEnemy>(body)));
	}

	for (int i = 0; i < 50; i++)
	{
		auto body = RigidBody(new PolygonShape(GameProcess::boatV,5), 0, 0);
		mobileEnemies.push_back((std::make_unique<MobileEnemy>(body)));
	}
}



PoolManager* PoolManager::getInstance()
{

	if (instance == nullptr)
	{
		instance = new PoolManager();
	}
	return instance;
	
}


void PoolManager::launchAvailableCannonBall(glm::vec2 startingPos, glm::vec2 dir, float speed,int layer)//Launch a cannonBall
{
	for (auto it = cannonBalls.begin(); it != cannonBalls.end(); it++)
	{
		if (!(*it)->active)
		{
			(*it)->launch(startingPos,dir,speed);
			(*it)->layer = layer;
			
			GameProcess::activeBalls.push_back(it->get());
			return;
		}
	}

	std::cout << "Erreur, aucun boulet n'est disponible\n";

}
void PoolManager::spawnAvailableBaseEnemy(glm::vec2 startingPos, float angle)//Spawn a base enemy
{
	for (auto it = baseEnemies.begin(); it != baseEnemies.end(); it++)
	{
		if (!(*it)->active)
		{
			(*it)->body->position = startingPos;
			(*it)->body->angle = angle;
			(*it)->active = true;
			GameProcess::activeEnemies.push_back(it->get());
			return;
		}
	}
}


void PoolManager::spawnAvailableMobileEnemy(glm::vec2 startingPos, float angle)//Spawn a mobile enemy
{
	for (auto it = mobileEnemies.begin(); it != mobileEnemies.end(); it++)
	{
		if (!(*it)->active)
		{
			(*it)->body->position = startingPos;
			(*it)->body->angle = angle;
			(*it)->active = true;
			GameProcess::activeEnemies.push_back(it->get());
			return;
		}
	}
}

