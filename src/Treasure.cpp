#include "Treasure.h"
#include "Render.h"
#include "ModelManager.h"


// We use a simple CircleShape for collision detection as a treasure is a collectible, not a physical object
Treasure::Treasure(glm::vec2 position)
{
	body = RigidBody(new CircleShape(0.5f), position.x,position.y);
	model = ModelManager::treasure.get();
	body.angularVelocity = 1.07f;
}


void Treasure::render()
{
	//We render the body for debug purposes
	//Render::renderBody(&body);   
	
	Render::renderModel(model,body.position,body.angle,Render::seaHeight(body.position));
}
void Treasure::update()
{
	body.update();
}