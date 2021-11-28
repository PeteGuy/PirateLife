#include "Shapes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


// Shapes used for collision detection

Shape::Shape()
{
	
}




CircleShape::CircleShape(float radius) : Shape()
{
	// Default number of points to draw the circle (60 on the circumference and 1 for the radius line and another one to draw the final line) 
	nbPoints = 62; 
	this->radius = radius;
	type = ShapeType::Circle;
}

void CircleShape::setPoints()
{
	

	

	this->vecPoints.push_back(glm::vec2(0, 0));


	//Reversed the ordre but it doesn't really matter
	for (int i = nbPoints - 2; i >= 0; i--)
	{

		float t = i * (2 * glm::pi<float>()) / (nbPoints - 2);
		

		
		this->vecPoints.push_back(glm::vec2(radius * glm::cos(t), radius * glm::sin(t)));
		this->indices.push_back(nbPoints - i - 2);


	}
	

	this->indices.push_back(1);






}

glm::vec2 CircleShape::getDimensions()
{
	return glm::vec2(2 * radius, 2 * radius);
}

AABB CircleShape::getAABB(glm::vec2 position, glm::mat2 rotationMat)
{

	return AABB{glm::vec2(position.x-radius,position.y-radius),glm::vec2(position.x + radius,position.y + radius) };

}



AABB_Shape::AABB_Shape(float width, float height)
{
	nbPoints = 5;
	this->width = width;
	this->height = height;
	type = ShapeType::AABB;
}




void AABB_Shape::setPoints()
{
	
	this->vecPoints.push_back(glm::vec2(-width / 2, -height / 2));
	

	this->vecPoints.push_back(glm::vec2(-width / 2, height / 2));
	

	this->vecPoints.push_back(glm::vec2(width / 2, height / 2));
	

	this->vecPoints.push_back(glm::vec2(width / 2, -height / 2));
	



	this->indices.push_back(0);
	
	for (int i = 0; i < 4; i++)
	{
		this->indices.push_back(3-i);
	
	}
	
}

glm::vec2 AABB_Shape::getDimensions()
{
	return glm::vec2(width, height);
}

AABB AABB_Shape::getAABB(glm::vec2 position, glm::mat2 rotationMat)
{
	return AABB{ glm::vec2(position.x-this->width/2.0f,position.y-this->height/2.0f),glm::vec2(position.x + this->width / 2.0f,position.y + this->height / 2.0f) };
}

PolygonShape::PolygonShape(float vertices[],int nbPoints)
{
	// the vertices array has to be a serie of 2 dimensional vectors (no z axis)
	assert(nbPoints >= 3);// We want at least three points to form a polygone
	this->nbPoints = nbPoints+1; // we have to add one to render it fully 
	//int testarr[] = { 1,2,3,4,5 };
	this->indices.push_back(0);

	for (int i = 0; i < nbPoints*2; i++)
	{
		this->indices.push_back((nbPoints*2)-1 - i);

	}
	//this->points = std::vector<float>(vertices,vertices+(nbPoints*2));
	for (int i = 0; i < nbPoints; i++)
	{
		this->vecPoints.push_back(glm::vec2(vertices[i*2], vertices[(i*2) + 1]));
		this->vecPoints.push_back(glm::vec2((vertices[i * 2] + vertices[(i * 2 + 2)%(nbPoints*2)])/2, (vertices[(i * 2) + 1] + vertices[(i * 2 + 3) % (nbPoints * 2)])/2));
	}

	/*for (int i = 0; i < nbPoints * 2; i++)
	{
		std::cout << "indice" << i << " : " << indices[i] << "\n";
		std::cout << "x" << i << " : " << vecPoints[i].x << "\n";
		std::cout << "y" << i << " : " << vecPoints[i].y << "\n";
	}*/
	
	this->nbPoints = nbPoints * 2 + 1;


	
	type = ShapeType::Polygon;
	
	calculateCOM();
	setPoints();
	
	setNormals();
	//points
}

void PolygonShape::setPoints()
{
	
	//This method simply translate every point in order to have the origin be the center of mass
	for (auto it = vecPoints.begin(); it < vecPoints.end(); it ++)
	{
		*it = *it - com;
		
		//*it = *it - com.x;
		//*(it + 1) = *(it + 1) - com.y;
	}

	
	com = glm::vec2();
	

}

void PolygonShape::setPointsAndMidpoints()
{

}

glm::vec2 PolygonShape::getDimensions()
{
	return glm::vec2();
}

void PolygonShape::setNormals()
{
	/*for (auto it = points.begin(); it != points.end()-3; it+=3)
	{												// x values    
		normals.push_back(glm::normalize(glm::vec2(*(it + 3 + 1) - *(it + 1), -(*(it + 3) - *it))));
	}*/
}


void PolygonShape::calculateCOM()
{
	com = glm::vec2();
	//we want everypoint but one of them is repeated
	for (auto it = vecPoints.begin(); it < vecPoints.end(); it++)
	{
		
		com += *it;
		

	}

	com /= nbPoints - 1;
	//std::cout << "COM is at x = " << com.x << "," << com.y << "\n";
}

AABB PolygonShape::getAABB(glm::vec2 position,glm::mat2 rotationMat)
{
	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	glm::vec2 currentPoint;
	for (auto it = vecPoints.begin(); it != vecPoints.end(); it++)
	{
		currentPoint = rotationMat * glm::vec2(it->x,it->y) + position;
		if (currentPoint.x > maxX)
		{
			maxX = currentPoint.x;
		}
		if (currentPoint.x < minX)
		{
			minX = currentPoint.x;
		}
		if (currentPoint.y > maxY)
		{
			maxY = currentPoint.y;
		}
		if (currentPoint.y < minY)
		{
			minY = currentPoint.y;
		}
		//aPoints.push_back(A->rotationMat * aShape->vecPoints[i] + A->position);

	}


	return AABB{ glm::vec2(minX,minY),glm::vec2(maxX,maxY) };
}

