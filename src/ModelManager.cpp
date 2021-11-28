#include "ModelManager.h"


//Central point to get any model 
namespace ModelManager
{
	std::unique_ptr<Model> boat;
	std::unique_ptr<Model> cannon;
	std::unique_ptr<Model> cannonBall;
	std::unique_ptr<Model> basicEnemy;
	std::unique_ptr<Model> obstacle;
	std::unique_ptr<Model> treasure;
	std::unique_ptr<Model> seaPlane;

	void initialize()//We load Every single model
	{
		boat = std::make_unique<Model>();
		cannon = std::make_unique<Model>();
		cannonBall = std::make_unique<Model>();
		basicEnemy = std::make_unique<Model>();
		obstacle = std::make_unique<Model>();
		treasure = std::make_unique<Model>();
		seaPlane = std::make_unique<Model>();
		if (!boat->Load("meshes/Boat.obj"))
		{
			ConsoleWriteErr("boat model could not be loaded");
		}
		if (!cannon->Load("meshes/Cannon.obj"))
		{
			ConsoleWriteErr("cannon model could not be loaded");
		}
		if (!cannonBall->Load("meshes/Ball.obj"))
		{
			ConsoleWriteErr("cannonBall model could not be loaded");
		}
		if (!basicEnemy->Load("meshes/BasicEnemy.obj"))
		{
			ConsoleWriteErr("BasicEnemy model could not be loaded");
		}
		if (!obstacle->Load("meshes/Obstacle.obj"))
		{
			ConsoleWriteErr("Obstacle model could not be loaded");
		}
		if (!treasure->Load("meshes/TreasureChest.obj"))
		{
			ConsoleWriteErr("TreasureChest model could not be loaded");
		}
		if (!seaPlane->Load("meshes/SeaPlane.obj"))
		{
			ConsoleWriteErr("SeaPlane model could not be loaded");
		}
	}
}