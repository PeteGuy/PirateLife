#include "PhysicData.h"

//A simple namespace to do physics related calculation

namespace PhysicData
{
	MassData defaultMassData
	{
		20.0f,
		calculateSafeInverse(20.0f),
		0.0f,
		calculateSafeInverse(20.0f)
	};



	MaterialData defaultMatDat
	{
		20.0f,
		0.9f,
		0.1f,
		0.05f
	};

	float calculateSafeInverse(float mass)
	{
		return (mass != 0) ? 1 / mass : 0;
	}

	glm::mat2 getRotationMat(float angle)
	{
		return glm::mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
	}
}
