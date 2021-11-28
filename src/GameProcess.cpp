#include "GameProcess.h"
#include "Debug.h"
#include "PoolManager.h"
#include "Render.h"


namespace GameProcess
{
	std::vector<CannonBall*> activeBalls;
	std::vector<Enemy*> activeEnemies;
	std::unique_ptr<Player> player;
	std::vector<Manifold> manifolds;
	std::vector <std::unique_ptr<Treasure>> treasures;

	int enemyCount;
	bool isWon = false;
	bool isLost = false;

	float boatV[] =
	{
		-0.5f,0.0f,
		-0.5f,1.4f,
		0.0f,2.0f,
		0.5f,1.4f,
		0.5f,0.0f,

	};

	//The points for possible obstacle entity, not implemented in the current version of the project
	float obstaclePoints[] = 
	{
		-0.003292f,1.46367f,
		0.689413f,1.12902f,
		1.08231f,0.350141f,
		1.17643f,-0.33975f,
		0.79524f,-0.855506f,
		-0.007186f,-1.27897f,
		-0.859028f,-1.18986f,
		-1.24982f,-0.592833f,
		-1.12212f,0.2069f,
		-0.713417f,1.05805f,
	};

	

	void initialize()
	{
		loadLevel();
	}


	void completeUpdate()
	{
		//We check for victory or defeat
		if (enemyCount <= 0)
		{
			isWon = true;
		}
		if (isWon || isLost)
		{
			return;
		}
		PvE_Solve();
		PvB_Solve();
		EvE_Solve();
		EvB_Solve();
		PvT_Solve();

		//now update everything
		player->update();
		if (player->hasLost)
		{
			isLost = true;
		}
		
		//For every entity, we first verify it is active and then update it 
		int i = 0;
		while (i < activeBalls.size())
		{
			activeBalls[i]->update();
			if (!activeBalls[i]->active)//This entity is inactive, its pointer is given back to the pool manager
			{
				activeBalls.erase(activeBalls.begin() + i);
				continue;
			}

			i++;
		}
		i = 0;
		while (i < activeEnemies.size())
		{
			activeEnemies[i]->update();
			if (!activeEnemies[i]->active)
			{
				activeEnemies.erase(activeEnemies.begin() + i);
				continue;
			}

			i++;
		}
		
	}

	void completeRender()
	{
		
		Render::renderSeaPlane();
		if (isWon)
		{
			Debug::log("congratualtion, you won");
		}
		else if (isLost)
		{
			Debug::log("game over");
		}
		player->render();
		int i = 0;
		while (i < activeBalls.size())
		{
			activeBalls[i]->render();
			if (!activeBalls[i]->active)
			{
				activeBalls.erase(activeBalls.begin() + i);
				continue;
			}

			i++;
		}

		i = 0;
		while (i < activeEnemies.size())
		{
			activeEnemies[i]->render();

			if (!activeEnemies[i]->active)
			{
				activeEnemies.erase(activeEnemies.begin() + i);
				continue;
			}

			Render::drawDirectionLine(player->boat->position, activeEnemies[i]->body->position);
			i++;
		}

		for (auto it = treasures.begin(); it != treasures.end(); it++)
		{
			(*it)->render();
		}


	}

	//Player VS Enemies update
	void PvE_Solve()
	{

		
		//We first check AABB collisions and then try more expensive collision detection

	
		for (auto et = activeEnemies.begin(); et != activeEnemies.end(); et++)
		{
			if(quickAABBvsAABB(player->boat->getDefaultAABB(), (*et)->body->getDefaultAABB()))
			{
				Manifold m{ player->boat,(*et)->body.get() };
				if(collisionFunctions[(int)m.A->shape->type][(int)m.B->shape->type](&m))
				{
					actOnCollision(&m);
				}
			}
		}



	}
	void PvB_Solve()
	{

	
		for (auto bt = activeBalls.begin(); bt != activeBalls.end(); bt++)
		{
			
			if (!(player->layer & (*bt)->layer) && quickAABBvsAABB(player->boat->getDefaultAABB(), (*bt)->body->getDefaultAABB()))
			{
				
				
				player->takeDamage(20);
				(*bt)->active = false;
				(*bt)->active = false;
				
			}
		}

	
	}
	void EvE_Solve()
	{
		manifolds.clear();
		//BroadPhase
		for (auto at = activeEnemies.begin(); at != activeEnemies.end(); at++)
		{
			for (auto bt = activeEnemies.begin(); bt != activeEnemies.end(); bt++)
			{
				if (at == bt)
				{
					continue;
				}
				//if A and B are on the same layer then don't collide them (not fully implemented for now)
				

				if (quickAABBvsAABB((*at)->body->getDefaultAABB(), (*bt)->body->getDefaultAABB()))
				{

					manifolds.push_back(
						{
							(*at)->body.get(),
							(*bt)->body.get(),



						});
				}
			}
		}
		for (int i =0; i <manifolds.size(); i++)
		{
			int j = i + 1;
			while (j < manifolds.size())
			{
				if (manifolds[i].A == manifolds[j].B)
				{
					if (manifolds[i].B == manifolds[j].A)
					{

						manifolds.erase(manifolds.begin()+j);


						continue;
					}
				}

				j++;
			}
		}



		//NarrowPhase

		for (auto m = manifolds.begin(); m < manifolds.end(); m++)
		{
			if(collisionFunctions[(int)(*m).A->shape->type][(int)(*m).B->shape->type](&(*m)))
			{
				actOnCollision(&(*m));

			}

		}


	}
	void EvB_Solve()
	{

		for (auto et = activeEnemies.begin(); et != activeEnemies.end(); et++)
		{
			
			for (auto bt = activeBalls.begin(); bt != activeBalls.end(); bt++)
			{
				if (!((*et)->layer & (*bt)->layer) && quickAABBvsAABB((*et)->body->getDefaultAABB(), (*bt)->body->getDefaultAABB()))
				{
					Manifold m{ (*et)->body.get(),(*bt)->body };
					(*et)->takeDamage(20);
					(*bt)->active = false;
			
				}
			}
		}
		
	}
	void PvT_Solve()
	{
		int i = 0;
		while (i < treasures.size())
		{
			treasures[i]->update();

			if (quickAABBvsAABB(player->boat->getDefaultAABB(), treasures[i]->body.getDefaultAABB()))
			{
				treasures.erase(treasures.begin() + i);
				player->addCannon();
				continue;
				
			}

			//Render::drawDirectionLine(player->boat->position, activeEnemies[i]->body->position);
			i++;
		}
		
	}


	void loadLevel()
	{
		player = std::make_unique<Player>(new RigidBody(new PolygonShape(boatV, 5), 0, 0), InputManager());
		player->reset();


		float pi = glm::pi<float>();

		for (int i = 0; i < 6; i++)
		{
			PoolManager::getInstance()->spawnAvailableBaseEnemy(glm::vec2(20*glm::cos((i*2*pi)/6), 20*glm::sin((i * 2 * pi) / 6)), (i * 2 * pi) / 6);
			
			enemyCount++;
		}
		for (int i = 0; i < 3; i++)
		{
			PoolManager::getInstance()->spawnAvailableMobileEnemy(glm::vec2(30 * glm::cos((i * 2 * pi) / 3), 30 * glm::sin((i * 2 * pi) / 3)), (i * 2 * pi) / 3);
			enemyCount++;
		}
		






	}


}