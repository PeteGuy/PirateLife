#include "Collision.h"




bool quickAABBvsAABB( AABB a,  AABB b)
{
	// Exit with no intersection if found separated along an axis
	if (a.max.x < b.min.x || a.min.x > b.max.x)
	{

		return false;
	}
	if (a.max.y < b.min.y || a.min.y > b.max.y)
	{
		return false;
	}
		

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}


//Collision functions

CollisionFunction collisionFunctions[(int)ShapeType::last][(int)ShapeType::last] =
{
  {
	resolveCollisionCircleCircle, resolveCollisionCircle_AABB,resolveCollisionCirclePolygon
  },
  {
	resolveCollisionAABB_Circle, resolveCollisionAABB_AABB
  },
	 {
	resolveCollisionPolygonCircle,resolveCollisionPolygon_AABB,resolveCollisionPolygonPolygon
  },
};


bool resolveCollisionPolygon_AABB(Manifold* m)
{
	return false;
}

bool resolveCollisionCircleCircle(Manifold* m)
{
	// Setup a couple pointers to each object
	RigidBody* A = m->A;
	RigidBody* B = m->B;

	CircleShape* aShape = dynamic_cast<CircleShape*>(A->shape);
	CircleShape* bShape = dynamic_cast<CircleShape*>(B->shape);

	// Vector from A to B
	glm::vec2 n = B->position - A->position;

	//Sum of the two radius
	float r = aShape->radius + bShape->radius;

	//No collision
	if (glm::length2(n) > r * r)
	{
		return false;
	}


	// Circles have collided, now compute manifold
	float d = glm::length(n); // perform actual sqrt

	// If distance between circles is not zero
	if (d != 0)
	{
		// Distance is difference between radius and distance
		m->penetration = r - d;

		// Utilize our d since we performed sqrt on it already within Length( )
		// Points from A to B, and is a unit vector
		m->normal = n * (1 / d);

		return true;
	}

	// Circles are on same position
	else
	{
		// Choose random (but consistent) values
		m->penetration = bShape->radius;
		m->normal = glm::vec2(1, 0);
		return true;
	}
}





bool resolveCollisionAABB_AABB(Manifold* m)
{

	// Setup a couple pointers to each object
	RigidBody* A = m->A;
	RigidBody* B = m->B;

	AABB_Shape* aShape = dynamic_cast<AABB_Shape*>(A->shape);
	AABB_Shape* bShape = dynamic_cast<AABB_Shape*>(B->shape);

	// Vector from A to B
	glm::vec2 n = A->position - B->position;

	

	// Calculate half extents along x axis for each object
	float a_extent = aShape->width / 2;
	float b_extent = bShape->width / 2;

	// Calculate overlap on x axis
	float x_overlap = a_extent + b_extent - abs(n.x);

	// SAT test on x axis
	if (x_overlap > 0)
	{
		// Calculate half extents along y axis for each object
		float a_extent = aShape->height / 2;
		float b_extent = bShape->height / 2;

		// Calculate overlap on y axis
		float y_overlap = a_extent + b_extent - abs(n.y);


		// SAT test on y axis
		if (y_overlap > 0)
		{
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap)
			{
				// Point towards B knowing that n points from A to B
				if (n.x < 0)
				{
					m->normal = glm::vec2(1, 0);
				}

				else
				{
					m->normal = glm::vec2(-1, 0);
				}

				m->penetration = x_overlap;

				return true;
			}
			else
			{
				// Point toward B knowing that n points from A to B
				if (n.y < 0)
				{
					m->normal = glm::vec2(0, 1);
				}

				else
				{
					m->normal = glm::vec2(0, -1);
				}

				m->penetration = y_overlap;
				return true;
			}
		}
	}

	return false;
}
bool resolveCollisionAABB_Circle(Manifold* m)
{
	// Setup a couple pointers to each object (le A doit être le rectangle et le B le Circle)
	RigidBody* A = m->A;
	RigidBody* B = m->B;

	AABB_Shape* aShape = dynamic_cast<AABB_Shape*>(A->shape);
	CircleShape* bShape = dynamic_cast<CircleShape*>(B->shape);

	// Vector from A to B
	glm::vec2 n = B->position - A->position;

	// Closest point on A to center of B
	glm::vec2 closest = n;

	// Calculate half extents along each axis
	float x_extent = aShape->width / 2;
	float y_extent = aShape->height / 2;

	// Clamp point to edges of the AABB
	closest.x = glm::clamp(closest.x, -x_extent, x_extent);
	closest.y = glm::clamp(closest.y, -y_extent, y_extent);

	bool inside = false;

	// Circle is inside the AABB, so we need to clamp the circle's center
	// to the closest edge
	if (n == closest)
	{

		inside = true;

		// Find closest axis
		if (abs(n.x) > abs(n.y))
		{
			// Clamp to closest extent
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}

		// y axis is shorter
		else
		{
			// Clamp to closest extent
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}

	}

	glm::vec2 normal = n - closest;
	float d = glm::length2(normal);
	float r = bShape->radius;

	// Early out of the radius is shorter than distance to closest point and
	// Circle not inside the AABB
	if (d > r * r && !inside)
		return false;


	// Avoided sqrt until we needed
	d = sqrt(d);

	// Collision normal needs to be flipped to point outside if circle was
	// inside the AABB
	if (inside)
	{
		//m->normal = (Vector2d)(-normal) * (1 / d);
		m->normal = (-normal) * (1 / d);
		m->penetration = r - d;
	}
	else
	{
		m->normal = normal * (1 / d);
		m->penetration = r - d;
	}
	return true;
}


bool resolveCollisionCircle_AABB(Manifold* m)
{
	// Setup a couple pointers to each object (le A doit être le rectangle et le B le Circle)
	RigidBody* A = m->A;
	RigidBody* B = m->B;

	
	CircleShape* aShape = dynamic_cast<CircleShape*>(A->shape);
	AABB_Shape* bShape = dynamic_cast<AABB_Shape*>(B->shape);

	// Vector from A to B
	glm::vec2 n = B->position - A->position;

	// Closest point on A to center of B
	glm::vec2 closest = n;

	// Calculate half extents along each axis
	float x_extent = bShape->width / 2;
	float y_extent = bShape->height / 2;

	// Clamp point to edges of the AABB
	closest.x = glm::clamp(closest.x, -x_extent, x_extent);
	closest.y = glm::clamp(closest.y, -y_extent, y_extent);

	bool inside = false;

	// Circle is inside the AABB, so we need to clamp the circle's center
	// to the closest edge
	if (n == closest)
	{

		inside = true;

		// Find closest axis
		if (abs(n.x) > abs(n.y))
		{
			// Clamp to closest extent
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}

		// y axis is shorter
		else
		{
			// Clamp to closest extent
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}

	}

	glm::vec2 normal = n - closest;
	float d = glm::length2(normal);
	float r = aShape->radius;

	// Early out of the radius is shorter than distance to closest point and
	// Circle not inside the AABB
	if (d > r * r && !inside)
		return false;


	// Avoided sqrt until we needed
	d = sqrt(d);

	// Collision normal needs to be flipped to point outside if circle was
	// inside the AABB
	if (inside)
	{
		//m->normal = (Vector2d)(-normal) * (1 / d);
		m->normal = (-normal) * (1 / d);
		m->penetration = r - d;
	}
	else
	{
		m->normal = normal * (1 / d);
		m->penetration = r - d;
	}
	return true;
}

bool resolveCollisionPolygonCircle(Manifold* m)
{
	auto aShape = dynamic_cast<PolygonShape*>(m->A->shape);
	auto bShape = dynamic_cast<CircleShape*>(m->B->shape);


	
	int aSize = aShape->nbPoints - 1;
	//int bSize = bShape->nbPoints - 1;
	std::vector<glm::vec2> aPoints;
	//std::vector<glm::vec2> bPoints;
	// 
	//On récupère les points dans l'espace
	for (int i = 0; i < aSize; i++)
	{
		aPoints.push_back(m->A->rotationMat * aShape->vecPoints[i] + m->A->position);

	}
	/*for (int i = 0; i < bSize; i++)
	{
		bPoints.push_back(B->rotationMat * bShape->vecPoints[i] + B->position);

	}*/
	//std::cout << "during that test aPoints contains" << aPoints.size() << "\n";
	//std::cout << "during that test aPoints contains" << bPoints.size() << "\n";
	//std::cout << "during that test asize is" << aSize << "\n";
	//std::cout << "during that test bsize is" << bSize << "\n";
	float distanceToCircleCenter = FLT_MAX;
	glm::vec2 closestPointToCircleCenter;

	for (auto at = aPoints.begin(); at != aPoints.end(); at++)
	{
		if (glm::length2(*at - m->B->position) < distanceToCircleCenter)
		{
			distanceToCircleCenter = glm::length2(*at - m->B->position);
			closestPointToCircleCenter = *at;


		}
	}

	if (glm::length2(closestPointToCircleCenter - m->B->position) > bShape->radius * bShape->radius)
	{
		return false;
	}

	glm::vec2 currentAxis;
	//On passe par toutes les faces de A
	for (int i = 0; i < aSize; i++)
	{
		// la première vertex est celle d'indice i
		// la seconde est celle d'indice (i + 1) % aSize, on veut la seconde vertex de la face, le modulo permet de prendre en compte un indice = aSize ce qui n'est pas possible

		currentAxis.x = -(aPoints[(i + 1) % aSize].y - aPoints[i].y);
		currentAxis.y = (aPoints[(i + 1) % aSize].x - aPoints[i].x);


		//On obtient ainsi la normal à la face;
		float amin = FLT_MAX;
		float amax = -FLT_MAX;
		float bmin = FLT_MAX;
		float bmax = -FLT_MAX;
		float dot;

		for (int j = 0; j < aSize; j++)
		{

			dot = glm::dot(aPoints[j], currentAxis);

			if (dot > amax)
			{
				amax = dot;
			}
			if (dot < amin)
			{
				amin = dot;
			}
		}

		

		bmax = glm::dot(m->B->position, currentAxis) + bShape->radius;
		bmin = glm::dot(m->B->position, currentAxis) - bShape->radius;
		if ((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin))
		{
			continue;
			std::cout << "we continued\n";
		}
		else
		{
	
			return false;
		}

	}


	float distance = FLT_MAX;

	for (auto at = aPoints.begin(); at != aPoints.end(); at++)
	{

		if (glm::length2(*at - m->B->position) < distance)
		{
			distance = glm::length2(*at - m->B->position);
			m->contacts[0] = *at;

		}

	}
	m->contacts[1] = m->contacts[0] - m->B->position;

	m->penetration = bShape->radius - glm::length(m->contacts[1]);
	m->normal = glm::normalize(m->B->position - m->A->position);

	return true;
}

//The polygon collision detection uses the SAT and a crude point based method to determine penetration
bool resolveCollisionCirclePolygon(Manifold* m)
{
	RigidBody* tmp = m->A;
	m->A = m->B;
	m->B = tmp;

	return resolveCollisionPolygonCircle(m);
}
bool resolveCollisionPolygonPolygon(Manifold* m)
{
	auto aShape = dynamic_cast<PolygonShape*>(m->A->shape);
	auto bShape = dynamic_cast<PolygonShape*>(m->B->shape);

	int aSize = aShape->nbPoints - 1;
	int bSize = bShape->nbPoints - 1;
	std::vector<glm::vec2> aPoints;
	std::vector<glm::vec2> bPoints;
	//On récupère les points dans l'espace
	for (int i = 0; i < aSize; i++)
	{
		aPoints.push_back(m->A->rotationMat * aShape->vecPoints[i] + m->A->position);

	}
	for (int i = 0; i < bSize; i++)
	{
		bPoints.push_back(m->B->rotationMat * bShape->vecPoints[i] + m->B->position);

	}
	//std::cout << "during that test aPoints contains" << aPoints.size() << "\n";
	//std::cout << "during that test aPoints contains" << bPoints.size() << "\n";
	//std::cout << "during that test asize is" << aSize << "\n";
	//std::cout << "during that test bsize is" << bSize << "\n";

	glm::vec2 currentAxis;
	//On passe par toutes les faces de A
	for (int i = 0; i < aSize; i++)
	{
		// la première vertex est celle d'indice i
		// la seconde est celle d'indice (i + 1) % aSize, on veut la seconde vertex de la face, le modulo permet de prendre en compte un indice = aSize ce qui n'est pas possible

		currentAxis.x = -(aPoints[(i + 1) % aSize].y - aPoints[i].y);
		currentAxis.y = (aPoints[(i + 1) % aSize].x - aPoints[i].x);


		//On obtient ainsi la normal à la face;
		float amin = FLT_MAX;
		float amax = -FLT_MAX;
		float bmin = FLT_MAX;
		float bmax = -FLT_MAX;
		float dot;

		for (int j = 0; j < aSize; j++)
		{

			dot = glm::dot(aPoints[j], currentAxis);

			if (dot > amax)
			{
				amax = dot;
			}
			if (dot < amin)
			{
				amin = dot;
			}
		}
		for (int j = 0; j < bSize; j++)
		{
			dot = glm::dot(bPoints[j], currentAxis);
			if (dot > bmax)
			{
				bmax = dot;
			}
			if (dot < bmin)
			{
				bmin = dot;
			}
		}
		if ((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin))
		{
			continue;
		}
		else
		{

			return false;
		}

	}


	for (int i = 0; i < bSize; i++)
	{
		// la première vertex est celle d'indice i
		// la seconde est celle d'indice (i + 1) % aSize, on veut la seconde vertex de la face, le modulo permet de prendre en compte un indice = aSize ce qui n'est pas possible

		currentAxis.x = -(bPoints[(i + 1) % bSize].y - bPoints[i].y);
		currentAxis.y = (bPoints[(i + 1) % bSize].x - bPoints[i].x);
		//On obtient ainsi la normal à la face;
		float amin = FLT_MAX;
		float amax = -FLT_MAX;
		float bmin = FLT_MAX;
		float bmax = -FLT_MAX;
		float dot;

		for (int j = 0; j < aSize; j++)
		{
			dot = glm::dot(aPoints[j], currentAxis);
			if (dot > amax)
			{
				amax = dot;
			}
			if (dot < amin)
			{
				amin = dot;
			}
		}
		for (int j = 0; j < bSize; j++)
		{
			dot = glm::dot(bPoints[j], currentAxis);
			if (dot > bmax)
			{
				bmax = dot;
			}
			if (dot < bmin)
			{
				bmin = dot;
			}
		}
		if ((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin))
		{
			continue;
		}
		else
		{
			return false;
		}

	}

	float distance = FLT_MAX;

	for (auto at = aPoints.begin(); at != aPoints.end(); at++)
	{
		for (auto bt = aPoints.begin(); bt != aPoints.end(); bt++)
		{
			if (glm::length2(*at - *bt) < distance)
			{
				distance = glm::length2(*at - *bt);
				m->contacts[0] = *at;
				m->contacts[1] = *bt;
			}
		}
	}

	m->penetration = glm::length(m->contacts[0] - m->contacts[1]);
	m->normal = glm::normalize(m->B->position - m->A->position);


	return true;

}

void actOnCollision(Manifold* m)
{
	// Pointers to our objects
	RigidBody* A = m->A;
	RigidBody* B = m->B;

	// Calculate relative velocity
	glm::vec2 rv = B->velocity - A->velocity;

	// Calculate relative velocity in terms of the normal direction
	float velAlongNormal = glm::dot(rv,m->normal); 	//rv.dotProduct(m->normal);

	

	// Do not resolve if velocities are separating
	if (velAlongNormal > 0)
	{
		// Here 1 is an arbitrary value
		if (glm::length2(rv) >= 1)
		{
			//Positional correction
			positionalCorrection(m);
			return;
		}

	}
	

	// Calculate restitution
	float e = std::min(A->restitution, B->restitution);

	// Calculate impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= A->inv_mass + B->inv_mass;

	
	// Apply impulse
	glm::vec2 impulse = m->normal * j;

	//Apply impulse to object velocity
	A->velocity += (-impulse * A->inv_mass);
	B->velocity += (impulse * B->inv_mass);
	
	//We recalculate relative velocity
	rv = B->velocity - A->velocity;

	// Calculate normalized tangent vector 
	glm::vec2 t = rv -  m->normal * glm::dot(rv, m->normal);

	if (glm::length2(t) == 0)
	{
		positionalCorrection(m);
		return;
	}


	t = glm::normalize(t);
	
	
	
	
	float velAlongTangent = glm::dot(rv, t);
	float jt = -(1 + e) * velAlongTangent;
	jt /= A->inv_mass + B->inv_mass;

	//Now we apply Coulomp law, this part is optional and could be replaced with velocity decrease
	float mu = (A->staticFriction + B->staticFriction) / 2;// We do a simple average

	glm::vec2 frictionImpulse;
	if (std::abs(jt) < mu * j)
	{
		
		frictionImpulse = jt * t;
	}
	else
	{
		float dynamicFriction = (A->dynamicFriction + B->dynamicFriction) / 2;
		frictionImpulse = -j * t * dynamicFriction;
	}

	A->velocity += (-frictionImpulse * A->inv_mass);
	B->velocity += (frictionImpulse * B->inv_mass);
	

	//Positional correction
	positionalCorrection(m);

	
}





void positionalCorrection(Manifold* m)
{
	//If the collision is to important, we manually modify their position
	const float percent = 0.3f; // usually 20% to 80%
	const float slop = 0.05f; // usually 0.01 to 0.1
	glm::vec2 correction = m->normal * (percent * glm::max(m->penetration - slop, 0.0f) / (m->A->inv_mass + m->B->inv_mass));
	m->A->position += -correction * m->A->inv_mass;
	m->B->position += correction * m->B->inv_mass;
}


bool PolygonVSPolygonSAT(RigidBody* A, RigidBody* B)
{
	auto aShape = dynamic_cast<PolygonShape*>(A->shape);
	auto bShape = dynamic_cast<PolygonShape*>(B->shape);

	int aSize = aShape->nbPoints - 1;
	int bSize = bShape->nbPoints - 1;
	std::vector<glm::vec2> aPoints;
	std::vector<glm::vec2> bPoints;
	//On récupère les points dans l'espace
	for (int i = 0; i < aSize; i++)
	{
		aPoints.push_back(A->rotationMat * aShape->vecPoints[i]+A->position);
		
	}
	for (int i = 0; i < bSize; i++)
	{
		bPoints.push_back(B->rotationMat * bShape->vecPoints[i] + B->position);
		
	}


	glm::vec2 currentAxis;
	//On passe par toutes les faces de A
	for (int i = 0; i < aSize; i++)
	{
		// la première vertex est celle d'indice i
		// la seconde est celle d'indice (i + 1) % aSize, on veut la seconde vertex de la face, le modulo permet de prendre en compte un indice = aSize ce qui n'est pas possible
		
		currentAxis.x = -(aPoints[(i + 1) % aSize].y - aPoints[i].y);
		currentAxis.y = (aPoints[(i + 1) % aSize].x - aPoints[i].x);
		
		
		//On obtient ainsi la normal à la face;
		float amin = FLT_MAX;
		float amax = -FLT_MAX;
		float bmin = FLT_MAX;
		float bmax= -FLT_MAX;
		float dot;

		for (int j = 0; j < aSize; j++)
		{

			dot = glm::dot(aPoints[j], currentAxis);
			
			if (dot > amax)
			{
				amax = dot;
			}
			if (dot < amin)
			{
				amin = dot;
			}
		}
		for (int j = 0; j < bSize; j++)
		{
			dot = glm::dot(bPoints[j], currentAxis);
			if (dot > bmax)
			{
				bmax = dot;
			}
			if (dot < bmin)
			{
				bmin = dot;
			}
		}
		if ((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin))
		{
			continue;
		}
		else
		{

			return false;
		}

	}
	
	
	for (int i = 0; i < bSize; i++)
	{
		// la première vertex est celle d'indice i
		// la seconde est celle d'indice (i + 1) % aSize, on veut la seconde vertex de la face, le modulo permet de prendre en compte un indice = aSize ce qui n'est pas possible

		currentAxis.x = -(bPoints[(i + 1) % bSize].y - bPoints[i].y);
		currentAxis.y = (bPoints[(i + 1) % bSize].x - bPoints[i].x);
		//On obtient ainsi la normal à la face;
		float amin = FLT_MAX;
		float amax = -FLT_MAX;
		float bmin = FLT_MAX;
		float bmax = -FLT_MAX;
		float dot;

		for (int j = 0; j < aSize; j++)
		{
			dot = glm::dot(aPoints[j], currentAxis);
			if (dot > amax)
			{
				amax = dot;
			}
			if (dot < amin)
			{
				amin = dot;
			}
		}
		for (int j = 0; j < bSize; j++)
		{
			dot = glm::dot(bPoints[j], currentAxis);
			if (dot > bmax)
			{
				bmax = dot;
			}
			if (dot < bmin)
			{
				bmin = dot;
			}
		}
		if ((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin))
		{
			continue;
		}
		else
		{
			return false;
		}

	}




	return true;
}


void compute_PVP_Manifold(Manifold* m)
{
	RigidBody* A = m->A;
	RigidBody* B = m->B;
	auto aShape = dynamic_cast<PolygonShape*>(A->shape);
	auto bShape = dynamic_cast<PolygonShape*>(B->shape);

	int aSize = aShape->nbPoints - 1;
	int bSize = bShape->nbPoints - 1;
	std::vector<glm::vec2> aPoints;
	std::vector<glm::vec2> bPoints;

	//On récupère les points dans l'espace
	for (int i = 0; i < aSize; i++)
	{
		aPoints.push_back(A->rotationMat * aShape->vecPoints[i] + A->position);

	}
	for (int i = 0; i < bSize; i++)
	{
		bPoints.push_back(B->rotationMat * bShape->vecPoints[i] + B->position);

	}

	float distance = FLT_MAX;

	for (auto at = aPoints.begin(); at != aPoints.end(); at++)
	{
		for (auto bt = aPoints.begin(); bt != aPoints.end(); bt++)
		{
			if (glm::length2(*at - *bt) < distance)
			{
				distance = glm::length2(*at - *bt);
				m->contacts[0] = *at;
				m->contacts[1] = *bt;
			}
		}
	}
	
	m->penetration = glm::length(m->contacts[0] - m->contacts[1]);
	m->normal = glm::normalize(B->position - A->position);



}

void compute_PVC_Manifold(Manifold* m)
{
	RigidBody* A = m->A;
	RigidBody* B = m->B;
	auto aShape = dynamic_cast<PolygonShape*>(A->shape);
	auto bShape = dynamic_cast<CircleShape*>(B->shape);

	int aSize = aShape->nbPoints - 1;
	//int bSize = bShape->nbPoints - 1;
	std::vector<glm::vec2> aPoints;
	//std::vector<glm::vec2> bPoints;

	//On récupère les points dans l'espace
	for (int i = 0; i < aSize; i++)
	{
		aPoints.push_back(A->rotationMat * aShape->vecPoints[i] + A->position);

	}
	

	float distance = FLT_MAX;

	for (auto at = aPoints.begin(); at != aPoints.end(); at++)
	{
		
		if (glm::length2(*at - B->position) < distance)
		{
			distance = glm::length2(*at - B->position);
			m->contacts[0] = *at;
			
		}
		
	}
	m->contacts[1] = m->contacts[0]-B->position;

	m->penetration = bShape->radius - glm::length(m->contacts[1]);
	m->normal = glm::normalize(B->position - A->position);
}

bool PolygonVSCircleSAT(RigidBody* A, RigidBody* B)
{

	auto aShape = dynamic_cast<PolygonShape*>(A->shape);
	auto bShape = dynamic_cast<CircleShape*>(B->shape);


	if(glm::length2(B->position - A->position) < bShape->radius*bShape->radius)
	{
		return true;
	}
	int aSize = aShape->nbPoints - 1;
	std::vector<glm::vec2> aPoints;
	for (int i = 0; i < aSize; i++)
	{
		aPoints.push_back(A->rotationMat * aShape->vecPoints[i] + A->position);

	}
	float distanceToCircleCenter = FLT_MAX;
	glm::vec2 closestPointToCircleCenter;

	for (auto at = aPoints.begin(); at != aPoints.end(); at++)
	{
		if (glm::length2(*at - B->position) < distanceToCircleCenter)
		{
			distanceToCircleCenter = glm::length2(*at - B->position);
			closestPointToCircleCenter = *at;


		}
	}

	if (glm::length2(closestPointToCircleCenter - B->position) > bShape->radius * bShape->radius)
	{
		return false;
	}

	glm::vec2 currentAxis;
	//On passe par toutes les faces de A
	for (int i = 0; i < aSize; i++)
	{
		// la première vertex est celle d'indice i
		// la seconde est celle d'indice (i + 1) % aSize, on veut la seconde vertex de la face, le modulo permet de prendre en compte un indice = aSize ce qui n'est pas possible

		currentAxis.x = -(aPoints[(i + 1) % aSize].y - aPoints[i].y);
		currentAxis.y = (aPoints[(i + 1) % aSize].x - aPoints[i].x);


		//On obtient ainsi la normal à la face;
		float amin = FLT_MAX;
		float amax = -FLT_MAX;
		float bmin = FLT_MAX;
		float bmax = -FLT_MAX;
		float dot;

		for (int j = 0; j < aSize; j++)
		{

			dot = glm::dot(aPoints[j], currentAxis);

			if (dot > amax)
			{
				amax = dot;
			}
			if (dot < amin)
			{
				amin = dot;
			}
		}



		bmax = glm::dot(B->position, currentAxis)+bShape->radius;
		bmin = glm::dot(B->position,currentAxis)-bShape->radius;
		if ((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin))
		{
			continue;
		}
		else
		{
			return false;
		}

	}


	return true;

}