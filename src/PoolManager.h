#pragma once

#include "Macros.h"
#include "CannonBall.h"
#include "Enemy.h"


class PoolManager
{
public:
	
	static PoolManager* getInstance();
	void launchAvailableCannonBall(glm::vec2 startingPos, glm::vec2 dir, float speed,int layer);

	void spawnAvailableBaseEnemy(glm::vec2 startingPos, float angle);
	void spawnAvailableMobileEnemy(glm::vec2 startingPos, float angle);
	

private:
	std::vector<std::unique_ptr<CannonBall>> cannonBalls;
	std::vector<std::unique_ptr<BaseEnemy>> baseEnemies;
	std::vector<std::unique_ptr<MobileEnemy>> mobileEnemies;
	
	

protected:
	PoolManager();
	static PoolManager* instance;
	

};