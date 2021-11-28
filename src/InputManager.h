#pragma once
#include "Macros.h"

class InputManager
{
public:
	bool isKeyPressed(int key);
	glm::vec2 getTargetLoc();

	InputManager();
	int pushForwardKey = GLFW_KEY_SPACE;
	int leftSideImpulse = GLFW_KEY_A; //should be Q (azerty)
	int rightSideImpulse = GLFW_KEY_D; //should be D(azerty)
	int slowDownKey = GLFW_KEY_LEFT_SHIFT; 
	int fireKey = GLFW_KEY_T;
	int switchCannonLeft = GLFW_KEY_Q;
	int switchCannonRight = GLFW_KEY_E;
	//int slowDownKey = GLFW_KEY_LEFT_ALT; //should be Q(azerty)
	
	//const int scancode = glfwGetKeyScancode(GLFW_KEY_X);



private:
	

};