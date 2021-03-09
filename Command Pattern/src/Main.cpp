#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "Application/Application.h"
#include "Application/Input.h"

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit())
	return -1;

	window = glfwCreateWindow(Application::WIDTH, Application::HEIGHT, "Hello World", NULL, NULL);

	if (!window)
	{
	glfwTerminate();
	return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Application::Init();
	Input::Init(window);
	unsigned char* data = new unsigned char[Application::NUM_PIXELS_ELEMNTS];

	while (!glfwWindowShouldClose(window))
	{
		Application::Update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Input::Close();
	Application::Close();

	glfwTerminate();
	return 0;
}