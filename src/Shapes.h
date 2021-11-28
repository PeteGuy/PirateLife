#pragma once
#include "Macros.h"
#include <vector>

enum class ShapeType  { Circle,AABB,Polygon,last };


struct AABB
{
	glm::vec2 min;
	glm::vec2 max;
};

class Shape
{
public:
	int nbPoints;
	//std::vector<float> points;
	std::vector<glm::vec2> vecPoints;
	// Careful, uint only goes up to 255
	std::vector<GLuint> indices;

	ShapeType type;

	Shape();
	virtual void setPoints() = 0;
	virtual glm::vec2 getDimensions() = 0;
	virtual AABB getAABB(glm::vec2 position,glm::mat2 rotationMat) = 0;

	glm::vec2 com = glm::vec2(0,0);

	
};

class CircleShape : public Shape
{
public:
	CircleShape(float radius);
	float radius;
	void setPoints() override;
	glm::vec2 getDimensions() override;
	AABB getAABB(glm::vec2 position, glm::mat2 rotationMat) override;
	
};

class AABB_Shape : public Shape
{
public:
	AABB_Shape(float width, float height);
	float width;
	float height;
	void setPoints() override;
	glm::vec2 getDimensions() override;
	AABB getAABB(glm::vec2 position, glm::mat2 rotationMat) override;
	

};


class PolygonShape : public Shape
{
public:
	PolygonShape(float vertices[], int nbPoints);
	void setPoints() override;
	void setPointsAndMidpoints();
	glm::vec2 getDimensions() override;
	void setNormals();
	void calculateCOM();
	AABB getAABB(glm::vec2 position, glm::mat2 rotationMat) override;
	

	std::vector<glm::vec2> normals;
	
	
};

