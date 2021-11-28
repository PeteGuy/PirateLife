#pragma once
#include "Macros.h"
#include "RigidBody.h"
#include "ModelManager.h"
#include "Player.h"

enum class EnemyState{patrolling, pursuing,attacking};

class Enemy
{
public :
	virtual void takeDamage(float damage) = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void die() = 0;
	void renderLifeBar();
	bool active;
	std::unique_ptr<RigidBody> body;
	int layer;

protected:
	
	float health;
	float maxHealth;
	Model* model;
	glm::vec2 currentTargetPosition;
	
};





class BaseEnemy : public Enemy
{
public:
	BaseEnemy(RigidBody& b);
	void takeDamage(float damage) override;
	void update() override;
	void render() override;
	void die() override;

private:
	std::unique_ptr<Cannon> cannon;
	float detectionRadius;
	float fireTimer;
	float aimTimer;
	float baseFireTimer = 2.0f;
	float baseAimTimer = 2.0f;
	Player* playerPointer;
};

class MobileEnemy : public Enemy
{
public:
	MobileEnemy(RigidBody& b);
	void takeDamage(float damage) override;
	void update() override;
	void render() override;
	void die() override;
	EnemyState state;

private:
	std::unique_ptr<Cannon> cannon;
	float detectionRadius;
	float fireTimer;
	float aimTimer;
	float baseFireTimer = 2.0f;
	float baseAimTimer = 2.0f;
	Player* playerPointer;
	
	
};


class ObstacleEnemy : public Enemy
{
public :
	ObstacleEnemy(RigidBody& b);
	void takeDamage(float damage) override;
	void update() override;
	void render() override;
	void die() override;

protected:
	Model* model;
};

