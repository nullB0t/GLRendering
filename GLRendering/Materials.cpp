#include "Materials.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SOIL
#include <SOIL/SOIL.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include <iostream>
#include "Shader.h"
#include "Camera.h"

Materials::Materials()
{
}

void Materials::Execute()
{
	//GLfloat startTime = static_cast<GLfloat>(glfwGetTime());

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLRendering", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//-- Input Binding
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	SetCallbackFunctions(window);

	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);

	// Shader Program
	Shader lightingShader("Content/Shaders/Lightning_Materials.vert", "Content/Shaders/Lightning_Materials.frag");
	Shader lampShader("Content/Shaders/Lamp.vert", "Content/Shaders/Lamp.frag");

	// This vertex data has the normal vectors for each vertex already inside
	// We could calculate the normal vectors with the cross product if we needed to
	// But since the vertices are on the same plane it's easy to manually add them
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		do_movement();

		// ** Rendering commands here **
#pragma region Rendering commands

		// Calculate DeltaTime
		//GLfloat deltaTime = (GLfloat)glfwGetTime() - startTime;
		//startTime = static_cast<GLfloat>(glfwGetTime());

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		lightingShader.Use();
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
		glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z); // vertex shader calculates view position

		// Get/Set material uniform properties -- green plastic
		GLfloat shine = 0.25;
		GLint matAmbientLoc = glGetUniformLocation(lightingShader.Program, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(lightingShader.Program, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(lightingShader.Program, "material.specular");
		GLint matShineLoc = glGetUniformLocation(lightingShader.Program, "material.shininess");
		glUniform3f(matAmbientLoc, 0.0f, 0.0f, 0.0f);
		glUniform3f(matDiffuseLoc, 0.1f, 0.35f, 0.1f);
		glUniform3f(matSpecularLoc, 0.45f, 0.55f, 0.45f);
		glUniform1f(matShineLoc, shine * 128.0f);

		// Get/Set Lighting properties
		GLint lightAmbientLoc = glGetUniformLocation(lightingShader.Program, "light.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, "light.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(lightingShader.Program, "light.specular");
		glUniform3f(lightAmbientLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

		// Change light color over time, remove this and you get a green plastic mat
		glm::vec3 lightColor;
		lightColor.x = static_cast<float>(sin(glfwGetTime() * 1.0f));
		lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.5f));
		lightColor.z = static_cast<float>(sin(glfwGetTime() * 2.0f));
		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.8f); // Decrease the influence, not to expose it at a 100%
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Same color, just Lower influence
		glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);

		// Camera/View transformation
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Projection
		glm::mat4 projection;
		projection = glm::perspective<GLfloat>(camera.Zoom, static_cast<GLfloat>(WIDTH) / static_cast<GLfloat>(HEIGHT), 0.1f, 100.0f);

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw
		glBindVertexArray(containerVAO);
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0); // Unbind VAO

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Note: to make something rotate around it's center, use the cos in one axis and the sin in the other to contrarrest the motion and rotate
		// make sure to add some value to move the object a bit far from it's center
		// Change the light's position values over time (can be done anywhere in the game loop actually, but try to do it at least before using the light source positions)
		/*lightPos.x = 2.0f * cos(glfwGetTime());
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		lightPos.z = 2.0f * sin(glfwGetTime());*/

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

#pragma endregion

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	// De-allocate all resources
	glDeleteVertexArrays(1, &containerVAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void Materials::do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void Materials::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

void Materials::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<GLfloat>(yoffset));
}

void Materials::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // this bool variable is initially set to true
	{
		lastX = static_cast<GLfloat>(xpos);
		lastY = static_cast<GLfloat>(ypos);
		firstMouse = false;
	}

	GLfloat xoffset = static_cast<GLfloat>(xpos - lastX);
	// Screen coordinates go from top to bottom, but OpenGL has a bottom to top screen position
	GLfloat yoffset = static_cast<GLfloat>(lastY - ypos); // Reversed since y-coordinates range from bottom to top
	lastX = static_cast<GLfloat>(xpos);
	lastY = static_cast<GLfloat>(ypos);

	GLfloat sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

#pragma region Callback setup (with access to member variables)
void Materials::SetCallbackFunctions(GLFWwindow* window)
{
	GLFWCallbackWrapper::SetApplication(this);
	glfwSetCursorPosCallback(window, GLFWCallbackWrapper::MousePositionCallback);
	glfwSetKeyCallback(window, GLFWCallbackWrapper::KeyboardCallback);
	glfwSetScrollCallback(window, GLFWCallbackWrapper::ScrollCallback);
}

Materials* Materials::GLFWCallbackWrapper::s_application = nullptr;

void Materials::GLFWCallbackWrapper::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	s_application->MousePositionCallback(window, xpos, ypos);
}

void Materials::GLFWCallbackWrapper::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	s_application->KeyboardCallback(window, key, scancode, action, mods);
}

void Materials::GLFWCallbackWrapper::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	s_application->ScrollCallback(window, xoffset, yoffset);
}

void Materials::GLFWCallbackWrapper::SetApplication(Materials *application)
{
	GLFWCallbackWrapper::s_application = application;
}
#pragma endregion
