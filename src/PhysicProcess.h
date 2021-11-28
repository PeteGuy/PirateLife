#pragma once
#include "RigidBody.h"
#include "Collision.h"


// A namepsace to test the physic engine, it is not used in this project

namespace PhysicProcess
{
	void BroadPhase();
	void NarrowPhase();
	void applyGravity();

	extern std::vector<std::unique_ptr<RigidBody>> bodies;
	extern std::vector<Manifold> manifolds;
	extern glm::vec2 gravity;
	
}