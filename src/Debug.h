#pragma once

#include "Macros.h"






namespace Debug
{
	


	extern GLfloat baseLine[];
	void initialize();
	void drawDebugLine(glm::vec3 position,float length,float angle);
	void drawDebugLine(glm::vec2 pos1, glm::vec2 pos2, float zPosition = 0);
	void drawDebugVector(glm::vec2 origin, glm::vec2 v, float zPosition = 0);
	void log(const char* varname, glm::vec2 vec);
	void log(const char* varname,int entier);
	void log(const char* varname, float flottant);
	void log(string texte);
	
}