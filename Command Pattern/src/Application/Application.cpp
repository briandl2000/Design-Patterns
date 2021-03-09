#define GLFW_INCLUDE_NONE
#include "Application/Application.h"
#include "Application/Input.h"
#include "Application/CommandList.h"
#include "Application/Commands/DrawCommand.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"
#include <iostream>

namespace Application {

	namespace INTERNAL {
		static GLuint quadVAO;
		static GLuint quadVBO;
		static GLuint quadIBO;

		static GLuint shader;

		static GLuint texture;

		static CommandList commandList;

		static bool StartDrawing;
		static std::vector<glm::vec2> drawPoints;
		bool ctrl;
	}
	
	using namespace INTERNAL;

	void Init()
	{
		//Quad
		{
			float quadVertices[16]
			{
				-1., -1., 0., 1.,
				 1., -1., 1., 1.,
				 1.,  1., 1., 0.,
				-1.,  1., 0., 0.
			};
			unsigned int indices[6] =
			{
				0, 1, 2,
				0, 3, 2
			};

			glGenVertexArrays(1, &quadVAO);
			glBindVertexArray(quadVAO);

			glCreateBuffers(1, &quadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

			glGenBuffers(1, &quadIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}
		//shader
		{
			const char* vShaderCode = R"(
				#version 330 core
				layout (location = 0) in vec2 aPos;
				layout (location = 1) in vec2 aUV;

				out vec2 UV;

				void main()
				{
					gl_Position = vec4(aPos, 0., 1.0);
					UV = aUV;
				}
			)";
			const char* fShaderCode = R"(
				#version 330 core
				out vec4 FragColor;

				in vec2 UV;

				uniform sampler2D tex;
				
				void main()
				{
					FragColor = texture(tex, UV);
				}
			)";

			unsigned int vertex, fragment;
			// vertex shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vShaderCode, nullptr);
			glCompileShader(vertex);
			{
				GLint success;
				GLchar infoLog[1024];
				glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR: " << "vertex" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
			
			// fragment Shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, nullptr);
			glCompileShader(fragment);
			
			{
				GLint success;
				GLchar infoLog[1024];
				glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR: " << "fragment" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
			
			// shader Program
			shader = glCreateProgram();
			glAttachShader(shader, vertex);
			glAttachShader(shader, fragment);
			glLinkProgram(shader);

			glDeleteShader(vertex);
			glDeleteShader(fragment);
			{
				GLint success;
				GLchar infoLog[1024];
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::PROGRAM_LINKING_ERROR:" << "shader" <<"\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
		}
		//texture
		{
			data = new unsigned char[NUM_PIXELS_ELEMNTS];

			for (int y = 0; y < HEIGHT; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					int i = (x + y * WIDTH)*4;
					data[i + 0] = (static_cast<float>(x)/WIDTH)*255;
					data[i + 1] = (static_cast<float>(y)/HEIGHT)*255;
					data[i + 2] = 0;
					data[i + 3] = 0;
				}
			}

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glProgramUniform1i(shader, glGetUniformLocation(shader, "tex"), 0);

		}
	}

	void Update()
	{
		time++;
		//std::cout << "X: " << Input::GetMousePos().x << "Y: " << Input::GetMousePos().y << std::endl;



		if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_1))
		{
			StartDrawing = true;
			glm::vec2 pos = Input::GetMousePos();
			drawPoints.push_back(pos);
		}
		else
		{
			if (StartDrawing)
			{
				commandList.AddCommand(new DrawCommand(drawPoints, data));
				drawPoints.clear();
			}
			StartDrawing = false;
		}

		if (Input::GetKey(GLFW_KEY_LEFT_CONTROL) && Input::GetKey(GLFW_KEY_Z))
		{
			if (!ctrl)
			{
				if (Input::GetKey(GLFW_KEY_LEFT_SHIFT))
				{
					commandList.Redo();
				}
				else
				{
					commandList.Undo();
				}
				ctrl = true;
			}
		}
		else
		{
			ctrl = false;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glUseProgram(shader);
		
		glBindVertexArray(quadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Close()
	{
		glDeleteBuffers(1, &quadVBO);
		glDeleteBuffers(1, &quadIBO);
	}


}