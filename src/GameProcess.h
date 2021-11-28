#pragma once


#include "Player.h"
#include "Enemy.h"
#include "CannonBall.h"
#include "PoolManager.h"
#include "Collision.h"
#include "Treasure.h"

namespace GameProcess
{
	extern std::vector<CannonBall*> activeBalls;
	extern std::vector<Enemy*> activeEnemies;
	extern std::unique_ptr<Player> player;
	extern std::vector<Manifold> manifolds;
	extern std::vector <std::unique_ptr<Treasure>> treasures;
	extern float boatV[];


	void initialize();
	void completeUpdate();
	void completeRender();
	void PvE_Solve();
	void PvB_Solve();
	void EvE_Solve();
	void EvB_Solve();
	void PvT_Solve();
	//void createLevelPerimeter(float width, float height);
	void loadLevel();

	extern int enemyCount;
	extern bool isWon;
	extern bool isLost;





}