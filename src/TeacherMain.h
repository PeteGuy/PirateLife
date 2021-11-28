#pragma once
#include "types.h"
#include "FileUtil.h"
#include "CImage.h"
#include "3d/camera.h"
#include "3d/model.h"
#include "3d/font.h"
#include "3d/physics.h"



// Global variables
extern bool	gKeyboardStates[GLFW_KEY_LAST + 1];	// Keys states
extern bool	gMouseRightPressed;					// Mouse button state
extern bool	gMouseMiddlePressed;				// Mouse button state
extern bool	gMouseLeftPressed;					// Mouse button state
extern float	gMousePositionX;					// Mouse position (relative to the start position)
extern float	gMousePositionY;					// Mouse position (relative to the start position)
extern float	gMouseRelativeX;					// Mouse relative movement
extern float	gMouseRelativeY;					// Mouse relative movement







extern float			knotAngleX;
extern float			knotAngleY;
extern float			knotAngleZ;




GLfloat vertices_Test[];



void callback_error(int error, const char* description);


void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);


void callback_mouse(GLFWwindow* window, double xpos, double ypos);


void callback_mouseButton(GLFWwindow* window, int button, int action, int mods);


void callback_mouseWheel(GLFWwindow* window, double xoffset, double yoffset);


void InitInputs(GLFWwindow* pWindow);


void InitializeOpenGLView(GLFWwindow* pWindow);

void UpdateGameLogic(float deltaTimeInSecond);


void DrawGame(float truc);


GLFWwindow* InitializeEverything();


int main1();
