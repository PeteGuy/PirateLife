#pragma once
#include "Macros.h"

#include "FileUtil.h"
#include "CImage.h"
#include "3d/camera.h"
#include "3d/model.h"
#include "3d/font.h"
#include "3d/physics.h"
#include "RigidBody.h"



namespace Render
{
	extern float windowWidth;
	extern float windowHeight;
	extern Shader defaultShader;
	extern Camera defaultCamera;

	extern glm::vec3 lightPosition;
	extern glm::vec3 lightColor;

	extern float progressBar[];
	extern int pbIndices[];

	extern unsigned int progressBarVBO;
	extern unsigned int progressBarVAO;
	extern unsigned int progressBarEBO;
	extern Shader progressBarShader;
	extern Shader seaShader;
	//extern glm::mat4 projection;
	//Default shader

	extern float timeValue;

	extern GLFWwindow* renderWindow;

	void initialize();
	//void init();
	void terminate();
	void changeWindowSize(float width, float height);

	void callback_error(int error, const char* description);
	void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	void callback_mouse(GLFWwindow* window, double xpos, double ypos);
	void callback_mouseButton(GLFWwindow* window, int button, int action, int mods);
	void callback_mouseWheel(GLFWwindow* window, double xoffset, double yoffset);
	void renderProgressBar(float value,glm::vec2 position, glm::vec3 colorFull, glm::vec3 colorEmpty, float length, bool isVertical = false,float height = 0);

	void renderBody(RigidBody* body);
	void renderModel(Model* model, glm::vec2 position, float angle, float zPosition,float xAngle = 0,float yAngle = 0);
	float seaHeight( float x, float y, float time = 0);
	float seaHeight(glm::vec2 position, float time = 0);
	float xSeaAngle(glm::vec2 position);
	float ySeaAngle(glm::vec2 position);
	void drawAABB(AABB aabb);


	glm::vec2 ScreenToWorldCoordinates(glm::vec2 coords);
	//Render function that uses interpolation not in use for now
	//void renderBody(float alpha);

	void setNewMouseButtonCallBack(GLFWmousebuttonfun callback);

	void drawDirectionLine(glm::vec2 position, glm::vec2 objectivePosition);

	void renderSeaPlane();

}