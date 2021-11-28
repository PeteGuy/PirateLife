#include "CannonBall.h"
#include "Render.h"
#include "timeOperations.h"
#include "ModelManager.h"



CannonBall::CannonBall(RigidBody* b)
{
	body = b;
	active = false;
	defaultLifeSpan = 5.0f;
	lifeSpan = 0;
	model = ModelManager::cannonBall.get();
}

void CannonBall::launch(glm::vec2 startingPos, glm::vec2 dir, float speed)
{
	body->position = startingPos;
	body->velocity = speed * dir;
	active = true;
	lifeSpan = defaultLifeSpan;

}


void CannonBall::update()
{
	
	body->update();
	lifeSpan -= dtValue;
	if (lifeSpan <= 0)
	{
		active = false;
	}
}

void CannonBall::render()
{
	//Debug render
	//Render::renderBody(body);
	float zPosition = Render::seaHeight(body->position);
	Render::renderModel(model, body->position, body->angle, zPosition + 0.5f);


}