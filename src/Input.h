#pragma once
#include "Macros.h"

namespace Input
{
	// Global variables
	extern bool	gKeyboardStates[GLFW_KEY_LAST + 1];	// Keys states
	extern bool	gMouseRightPressed;					// Mouse button state
	extern bool	gMouseMiddlePressed;				// Mouse button state
	extern bool	gMouseLeftPressed;					// Mouse button state
	extern float	gMousePositionX;					// Mouse position (relative to the start position)
	extern float	gMousePositionY;					// Mouse position (relative to the start position)
	extern float	gMouseRelativeX;					// Mouse relative movement
	extern float	gMouseRelativeY;	
	
	extern float	gMouseScrollPositionX;					
	extern float	gMouseScrollPositionY;					
	extern float	gMouseScrollRelativePositionX;				
	extern float	gMouseScrollRelativePositionX;					
	extern float	gMouseScrollRelativePositionY;

	extern bool  bScrollFirstTime;

	extern float mouseScrollStartPosY;

	extern float mouseScrollLastPosY;



	void callback_error(int error, const char* description);
	void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	void callback_mouse(GLFWwindow* window, double xpos, double ypos);
	void callback_mouseButton(GLFWwindow* window, int button, int action, int mods);
	void callback_mouseWheel(GLFWwindow* window, double xoffset, double yoffset);
	


	void initialize(GLFWwindow* window);
	
}