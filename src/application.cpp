// application.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Run program: Ctrl + F5 or Menu Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
// Add/remove a breakpoint in the code : F9 on the line you want your breakpoint (the program must be built in debug and ran in debug).

// Tips for Getting Started: 
//   Use the Solution Explorer window to add/manage files
//   Use the Output window to see build output and other messages

// Docs :
// GLFW	  : https://www.glfw.org/docs/latest/modules.html						(Library to handle window, input, OpenGL contexts)
// glm	  : https://glm.g-truc.net/0.9.9/index.html								(Library for OpenGL math (vectors, matrices, etc))
// assimp : https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html	(Library to load 3D objects)

// exemple	: https://github.com/zhihui-xie/CG_OpenGL


//#include "TeacherMain.h"

#include "Render.h"
#include "Debug.h"
#include "RigidBody.h"
#include "InputManager.h"
#include "Player.h"
#include "Input.h"
#include "Collision.h"
#include "Cannon.h"
#include "CannonBall.h"
#include "3d/model.h"
#include "PoolManager.h"
#include <glm/gtc/matrix_inverse.hpp>
#include "ModelManager.h"
#include "GameProcess.h"
#include "CameraController.h"
#include "3d/font.h"
#if defined(WIN32)
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#endif
#include <chrono>

using namespace std::chrono_literals;

constexpr std::chrono::nanoseconds dt(16ms);
const float dtValue = 0.016f;


//On choisi un dt de 16ms (1/60fps)

std::chrono::nanoseconds accumulator(0ns);




int main()
{
	int previousScrollY = 0;
	int zoomLevel = 0;




	float boatV[] =
	{
		-0.5f,0.0f,
		-0.5f,1.4f,
		0.0f,2.0f,
		0.5f,1.4f,
		0.5f,0.0f,

	};

	bool canZoom = true;

	//RigidBody()
	Render::initialize();
	Debug::initialize();
	Input::initialize(Render::renderWindow);
	ModelManager::initialize();

	float camBaseZ = Render::defaultCamera.m_Position.z;
	GameProcess::initialize();
	PoolManager::getInstance();


	GLFWwindow* window = Render::renderWindow;
	Shader defaultShader = Render::defaultShader;




	CameraController cameraController(GameProcess::player.get(), &Render::defaultCamera);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	using clock = std::chrono::high_resolution_clock;
	auto frameStart = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();




	while (!glfwWindowShouldClose(window))
	{
		
		
		

		currentTime = std::chrono::high_resolution_clock::now();
		// Store the time elapsed since the last frame began
		accumulator += currentTime - frameStart;
		

		// Record the starting of this frame
		frameStart = currentTime;


		if (accumulator > std::chrono::microseconds(200ms))
		{
			accumulator = std::chrono::microseconds(200ms);
		}

		

		while (accumulator > dt)
		{
			

			//update everything
			
			GameProcess::completeUpdate();
			cameraController.followTarget();

			//Adjust camera zoom level
			float scroll = Input::gMouseScrollPositionY;
	
			if (scroll != previousScrollY)
			{
				if (zoomLevel == -1)
				{
					if (scroll > previousScrollY)
					{
						zoomLevel++;
					}
				}
				else if (zoomLevel == 8)
				{
					if (scroll < previousScrollY)
					{
						zoomLevel--;
					}
				}
				else
				{
					if (scroll > previousScrollY)
					{
						zoomLevel++;
					}
					else if (scroll < previousScrollY)
					{
						zoomLevel--;
					}
					//zoomLevel = scroll;
				}
			}
			previousScrollY = scroll;
			
			Render::defaultCamera.m_Position.z = camBaseZ - zoomLevel;
			
			
			
			
			Render::timeValue += dtValue;
			accumulator -= dt;
		}

		//We calculate the alpha for possible interpolation (not implemented yet)
		auto alpha = (float)accumulator.count() / dt.count();



		// Render part
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render everything
		
		GameProcess::completeRender();
		

		
		
		



		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	Render::terminate();

	return 0;
}