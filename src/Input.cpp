#include "Input.h"



namespace Input
{
	bool	gKeyboardStates[GLFW_KEY_LAST + 1];	// Keys states
	bool	gMouseRightPressed;					// Mouse button state
	bool	gMouseMiddlePressed;				// Mouse button state
	bool	gMouseLeftPressed;					// Mouse button state
	float	gMousePositionX;					// Mouse position (relative to the start position)
	float	gMousePositionY;					// Mouse position (relative to the start position)
	float	gMouseRelativeX;					// Mouse relative movement
	float	gMouseRelativeY;

	float	gMouseScrollPositionX;					// Mouse position (relative to the start position)
	float	gMouseScrollPositionY;					// Mouse position (relative to the start position)
	float	gMouseScrollRelativePositionX;					// Mouse relative movement
	float	gMouseScrollRelativePositionY;

	bool  bScrollFirstTime = true;

	float mouseScrollStartPosY = 0.0;

	float mouseScrollLastPosY = 0.0;

	void initialize(GLFWwindow* pWindow)
	{
		// Connect callback for input (keyboard, mouse), hide the mouse cursor.
		glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetKeyCallback(pWindow, callback_keyboard);
		glfwSetCursorPosCallback(pWindow, callback_mouse);
		glfwSetMouseButtonCallback(pWindow, callback_mouseButton);
		glfwSetScrollCallback(pWindow, callback_mouseWheel);

		for (size_t i = 0; i < sizeofarray(gKeyboardStates); i++)
		{
			gKeyboardStates[i] = false;
		}
		gMouseRightPressed = false;
		gMouseMiddlePressed = false;
		gMouseLeftPressed = false;
		gMouseRelativeX = 0.0f;
		gMouseRelativeY = 0.0f;
		gMouseScrollPositionX = 0.0f;					// Mouse position (relative to the start position)
		gMouseScrollPositionY = 0.0f;				// Mouse position (relative to the start position)
		gMouseScrollRelativePositionX = 0.0f;			// Mouse relative movement
		gMouseScrollRelativePositionY = 0.0f;
	}

	void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			return;
		}

		if (action == GLFW_PRESS)
		{
			gKeyboardStates[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			gKeyboardStates[key] = false;
		}

		
	}

	void callback_mouse(GLFWwindow* window, double xpos, double ypos)
	{
		static bool  bFirstTime = true;
		static float mouseStartPosX = 0.0;
		static float mouseStartPosY = 0.0;
		static float mouseLastPosX = 0.0;
		static float mouseLastPosY = 0.0;

		if (bFirstTime)
		{
			mouseStartPosX = 0.0f;
			mouseStartPosY = 0.0f;
			mouseLastPosX = (float)xpos;
			mouseLastPosY = -(float)ypos;
			bFirstTime = false;
		}

		gMousePositionX = ((float)xpos - mouseStartPosX);
		gMousePositionY = -((float)ypos - mouseStartPosY);
		gMouseRelativeX = gMousePositionX - mouseLastPosX;
		gMouseRelativeY = gMousePositionY - mouseLastPosY;
		mouseLastPosX = gMousePositionX;
		mouseLastPosY = gMousePositionY;

	}

	void callback_mouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		bool bIsButtonPressed = (action == GLFW_PRESS);
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			gMouseRightPressed = bIsButtonPressed;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			gMouseMiddlePressed = bIsButtonPressed;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			gMouseLeftPressed = bIsButtonPressed;
		}
	}

	void callback_mouseWheel(GLFWwindow* window, double xoffset, double yoffset)
	{

		

		if (bScrollFirstTime)
		{

			mouseScrollStartPosY = 0.0f;

			mouseScrollLastPosY = -(float)yoffset;
			bScrollFirstTime = false;

			gMouseScrollPositionY = 0.0f;
		}


		gMouseScrollPositionY += (float)yoffset;

		gMouseScrollRelativePositionY = (float)yoffset;


	}

	
}

