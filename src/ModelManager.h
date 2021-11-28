#pragma once

#include "Macros.h"
#include "3d/model.h"


namespace ModelManager
{
	extern std::unique_ptr<Model> boat;
	extern std::unique_ptr<Model> cannon;
	extern std::unique_ptr<Model> cannonBall;
	extern std::unique_ptr<Model> basicEnemy;
	extern std::unique_ptr<Model> obstacle;
	extern std::unique_ptr<Model> treasure;
	extern std::unique_ptr<Model> seaPlane;


	void initialize();

}