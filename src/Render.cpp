#pragma once

#include "Render.h"
#include "Debug.h"
#include "ModelManager.h"





// Namespace used to render objects from the game
namespace Render
{


	float windowWidth = 800;
	float windowHeight = 600;
	GLFWwindow* renderWindow;
	Shader defaultShader;
	Camera defaultCamera;
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 identity = glm::mat4(1.0f);

	glm::vec3 lightPosition = glm::vec3(0,0,100);
	glm::vec3 lightColor = glm::vec3(1,1,1);

	float timeValue = 0;

	float progressBar[] =
	{
		0.0f,0.5f,0.0f,
		1.0f,0.5f,0.0f,
		1.0f,-0.5f,0.0f,
		0.0f,-0.5f,0.0f
	};

	int pbIndices[] =
	{
		0,1,3,
		1,2,3
	};

	unsigned int progressBarVBO;
	unsigned int progressBarVAO;
	unsigned int progressBarEBO;
	Shader progressBarShader;
	Shader seaShader;


	void initialize()
	{
		// Initialize GLFW
		glfwSetErrorCallback(callback_error);
		if (!glfwInit())
		{
			return;
		}
		else
		{
			string version = glfwGetVersionString();
			ConsoleWriteOk("GLFW (window/input library) :");
			ConsoleWrite(" -> version : %s", version.c_str());
		}

		// Setup OpenGL with GLFW (we set OpenGL 3.3 minimum, window not resizable)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		renderWindow = glfwCreateWindow(windowWidth, windowHeight, "RenderWindow", NULL, NULL);
		// Error check if the window fails to create
		if (renderWindow == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();

		}
		glfwMakeContextCurrent(renderWindow);

		glewExperimental = GL_TRUE;
		GLenum glewErr = glewInit();
		if (glewErr != GLEW_OK)
		{
			ConsoleWriteOk("GLew error : %s", glewGetErrorString(glewErr));
			return;
		}
		else
		{
			string version = (const char*)glewGetString(GLEW_VERSION);
			ConsoleWriteOk("GLew (OpenGL extender) :");
			ConsoleWrite(" -> version : %s", version.c_str());
		}

		defaultShader.Load("shaders/default.vert","shaders/default.frag");
		defaultShader.Use();
		progressBarShader.Load("shaders/progressBar.vert", "shaders/progressBar.frag");

		seaShader.Load("shaders/seaShader.vert", "shaders/seaShader.frag");
		seaShader.Load("shaders/seaShader.vert", "shaders/seaShader.frag");


		// Setup caméra
		defaultCamera.SetPosition(0, 0, 10.0f);
		defaultCamera.SetSize(windowWidth, windowHeight);
		defaultCamera.SetViewAngle(90);
		
		glEnable(GL_DEPTH_TEST);



		glGenVertexArrays(1, &progressBarVAO);
		glGenBuffers(1, &progressBarVBO);
		glGenBuffers(1, &progressBarEBO);


		glBindVertexArray(progressBarVAO);
		glBindBuffer(GL_ARRAY_BUFFER, progressBarVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(progressBar), progressBar, GL_DYNAMIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, progressBarEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pbIndices), pbIndices, GL_DYNAMIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		
		return;
	}

	

	void terminate()
	{
		//delete &defaultShader;
		// Delete window before ending the program
		glfwDestroyWindow(renderWindow);
		// Terminate GLFW before ending the program
		glfwTerminate();
	}

	void changeWindowSize(float width, float height)
	{
		windowWidth = width;
		windowHeight = height;
		projection = glm::ortho(0.0f, Render::windowWidth, Render::windowHeight, 0.0f, -1.0f, 1.0f);
		glfwSetWindowSize(renderWindow, windowWidth, windowHeight);
		
	}

	

	void callback_error(int error, const char* description)
	{
		fprintf(stderr, "Error %d : %s\n", error, description);
	}


	void renderBody(RigidBody* body)
	{
		Render::defaultShader.Use();


		Render::defaultShader.SetUniform("projection", Render::defaultCamera.GetProjMatrix());


		model = glm::translate(identity, glm::vec3(body->position, 0.0f));
		model = glm::rotate(model, body->angle, glm::vec3(0.0f, 0.0f, 1.0f)); //originally used glm::radian(body->angle)
		Render::defaultShader.SetUniform("view", Render::defaultCamera.GetViewMatrix());
		Render::defaultShader.SetUniform("model", model);
		
		glBindVertexArray(body->vao);
		glDrawElements(GL_LINE_STRIP, body->shape->nbPoints, GL_UNSIGNED_INT, 0);


		if (COM_FLAG && body->shape->type == ShapeType::Polygon)
		{

			glBindVertexArray(body->COMvao);
			glDrawArrays(GL_POINTS, 0, 1);


		}
	}


	glm::vec2 ScreenToWorldCoordinates(glm::vec2 coords)
	{
		glm::mat4 screenToModel = glm::ortho(0.0f, windowWidth, -windowHeight, 0.0f, -0.1f, 0.1f);
		glm::mat4 worldProjection = defaultCamera.getBaseView() * defaultCamera.GetProjMatrix();
		glm::vec4 coords4 = glm::vec4(coords.x, coords.y, 0, 1);

		coords4 = glm::inverse(worldProjection) * screenToModel * coords4;
		float halhHeight = tan(glm::radians(defaultCamera.m_ViewAngle / 2.0f)) * defaultCamera.m_Position.z;

		

		
		return glm::vec2(coords4.x * halhHeight + defaultCamera.m_Position.x,coords4.y*halhHeight+defaultCamera.m_Position.y);

	}

	void renderProgressBar(float value, glm::vec2 position, glm::vec3 colorFull, glm::vec3 colorEmpty,float length,bool isVertical, float height)
	{
		
		float fullLength = length * value;
		float emptyLength = length * (1 - value);

		float largeurBar = 0.1f;
			

		glm::mat4 identity = glm::mat4(1.0f);
		progressBarShader.Use();

		progressBarShader.SetUniform("projection", Render::defaultCamera.GetProjMatrix());
		progressBarShader.SetUniform("view", Render::defaultCamera.GetViewMatrix());



		if (isVertical)
		{
			glm::mat4 model = glm::translate(identity, glm::vec3(position, height));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(fullLength, largeurBar, 1));

			progressBarShader.SetUniform("model", model);
			progressBarShader.SetUniform("currentColor", colorFull);

			glBindVertexArray(progressBarVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			model = glm::translate(identity, glm::vec3(position + fullLength * glm::vec2(0, 1),height));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(emptyLength, largeurBar, 1));

			progressBarShader.SetUniform("model", model);
			progressBarShader.SetUniform("currentColor", colorEmpty);

			glBindVertexArray(progressBarVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		else
		{
			glm::mat4 model = glm::translate(identity, glm::vec3(position, height));
			model = glm::scale(model, glm::vec3(fullLength, largeurBar, 1));

			progressBarShader.SetUniform("model", model);
			progressBarShader.SetUniform("currentColor", colorFull);

			glBindVertexArray(progressBarVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			model = glm::translate(identity, glm::vec3(position+fullLength*glm::vec2(1,0), height));
			model = glm::scale(model, glm::vec3(emptyLength, largeurBar, 1));

			progressBarShader.SetUniform("model", model);
			progressBarShader.SetUniform("currentColor", colorEmpty);

			glBindVertexArray(progressBarVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	
		
	}


	void drawDirectionLine(glm::vec2 position, glm::vec2 objectivePosition)
	{
		glm::vec2 drawVector = glm::normalize(objectivePosition - position);

		Debug::drawDebugLine(position + 3.0f * drawVector, position + 5.0f * drawVector,0.5f);


	}

	void renderModel(Model* model, glm::vec2 position, float angle, float zPosition, float xAngle , float yAngle)
	{
		
		
		glm::mat4 modelMat4 = glm::translate(identity, glm::vec3(0, 0, 0));
		modelMat4 = glm::translate(modelMat4, glm::vec3(position, zPosition));
		modelMat4 = glm::rotate(modelMat4, glm::radians(90.0f), glm::vec3(1, 0, 0));
		modelMat4 = glm::rotate(modelMat4, angle, glm::vec3(0, 1, 0));
		modelMat4 = glm::rotate(modelMat4, xAngle, glm::vec3(1, 0, 0));
		modelMat4 = glm::rotate(modelMat4, yAngle, glm::vec3(0, 0, 1));
		modelMat4 = glm::scale(modelMat4, glm::vec3(1, 1, 1));
		model->Draw(Render::defaultCamera, modelMat4, lightPosition, lightColor);
	}

	float seaHeight(float x, float y, float time)
	{
		if (time == 0)
		{
			return 0.25f*glm::cos(timeValue + x / 20 + y / 20);
		}
		else
		{
			return 0.25f*glm::cos(time + x / 20 + y / 20);
		}
		
	}

	float seaHeight(glm::vec2 position, float time)
	{
		return seaHeight(position.x, position.y, time);
	}

	float xSeaAngle(glm::vec2 position)
	{
		return 0.05f * glm::sin(timeValue + position.x / 100 + position.y / 100);
	}
	float ySeaAngle(glm::vec2 position)
	{
		
		return 0.1f * glm::cos(timeValue*2 + position.x / 20 + position.y / 20);
	}

	void renderSeaPlane()
	{
		glm::mat4 model = glm::translate(identity, glm::vec3(defaultCamera.m_Position.x, defaultCamera.m_Position.y, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		seaShader.Use();
		seaShader.SetUniform("timeValue", timeValue);
		seaShader.SetUniform("normalMatrix", normalMatrix);
		seaShader.SetUniform("model", model);
		seaShader.SetUniform("view", defaultCamera.GetViewMatrix());
		seaShader.SetUniform("proj", defaultCamera.GetProjMatrix());
		seaShader.SetUniform("lightColor", lightColor);
		seaShader.SetUniform("lightPosition", lightPosition);

		ModelManager::seaPlane->noShaderDraw();
		
	}

	void drawAABB(AABB aabb)
	{
		float width = aabb.max.x - aabb.min.x;
		float height = aabb.max.y - aabb.min.y;

		glm::mat4 identity = glm::mat4(1.0f);
		defaultShader.Use();
		defaultShader.SetUniform("projection", defaultCamera.GetProjMatrix());
		defaultShader.SetUniform("view", defaultCamera.GetViewMatrix());


		model = glm::translate(identity, glm::vec3(aabb.min.x,aabb.min.y + height/2, 0));
		model = glm::scale(model, glm::vec3(width, height, 1));

		defaultShader.SetUniform("model", model);

		glBindVertexArray(progressBarVAO);
		glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}




