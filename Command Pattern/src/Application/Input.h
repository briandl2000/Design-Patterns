#pragma once
#include "glm/vec2.hpp"
#include <GLFW/glfw3.h>

namespace Input {

	void Init(GLFWwindow* _window);
	void Close();

	bool GetKey(int keyCode);
	
	bool GetMouseButton(int button);
	glm::vec2 GetMousePos();

}