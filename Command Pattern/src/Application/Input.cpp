#include "Application/Input.h"

namespace Input {

	namespace Internal {
		static glm::vec2 MousePos;
		static bool keys[1000];
		static bool mouseButtons[8];
	}
	
	using namespace Internal;

	void Init(GLFWwindow* _window)
	{

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					keys[key] = true;
					break;
				}
				case GLFW_RELEASE:
				{
					keys[key] = false;
					break;
				}
				default:
					break;
			}
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					mouseButtons[button] = true;
					break;
				}
				case GLFW_RELEASE:
				{
					mouseButtons[button] = false;
					break;
				}
			}
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			MousePos.x = xPos;
			MousePos.y = yPos;
		});

	}
	
	void Close()
	{
	}
	
	bool GetKey(int keyCode)
	{
		return keys[keyCode];
	}
	
	bool GetMouseButton(int button)
	{
		return mouseButtons[button];
	}
	
	glm::vec2 GetMousePos()
	{
		return MousePos;
	}

}