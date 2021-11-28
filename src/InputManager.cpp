#include "InputManager.h"
#include "Input.h"
#include "Debug.h"


//Simple input system to handle player input
bool InputManager::isKeyPressed(int key)
{
	return Input::gKeyboardStates[key];

	//return Input::gKeyboardStates[key];
	
}

InputManager::InputManager()
{
	std::cout << "Input Manager created\n";
}



glm::vec2 InputManager::getTargetLoc()
{
	glm::vec2 targetLoc = glm::vec2(Input::gMousePositionX, Input::gMousePositionY);
	//Debug::log("target", targetLoc);
	return targetLoc;
}