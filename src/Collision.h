#pragma once
#include "RigidBody.h"

struct Manifold
{
	RigidBody* A;
	RigidBody* B;
	float penetration;
	glm::vec2 normal;
	glm::vec2 contacts[2];     // Points of contact during collision
	uint32_t contact_count;
};


void compute_PVP_Manifold(Manifold* m);
void compute_PVC_Manifold(Manifold* m);

bool PolygonVSPolygonSAT(RigidBody *A, RigidBody *B);
bool PolygonVSCircleSAT(RigidBody* A, RigidBody* B);


// This function serves in the broad phase
bool quickAABBvsAABB(AABB a, AABB b);

typedef bool (*CollisionFunction)(Manifold* m);

extern CollisionFunction collisionFunctions[(int)ShapeType::last][(int)ShapeType::last];

//Collision resolution function
bool resolveCollisionCircleCircle(Manifold* m);
bool resolveCollisionCircle_AABB(Manifold* m);
bool resolveCollisionAABB_AABB(Manifold* m);
bool resolveCollisionAABB_Circle(Manifold* m);

bool resolveCollisionPolygonCircle(Manifold* m);
bool resolveCollisionPolygonPolygon(Manifold* m);
bool resolveCollisionPolygon_AABB(Manifold* m);
bool resolveCollisionCirclePolygon(Manifold* m);


// Execute necessary action : changing the velocity of colliding objects to separate them
void actOnCollision(Manifold* m);
void positionalCorrection(Manifold* m);