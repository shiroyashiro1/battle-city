#include "ShaderProgram.h"
#include <iostream>

namespace Renderer
{
	ShaderProgram::ShaderProgram(const std::string& vertexShader,
		const std::string& fragmentShader)
	{
		GLuint vertexShaderID;
		if (createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
		{
			std::cerr << "\nVertex Shader Compile Time Error\n";
			return;
		}

		GLuint fragmentShaderID;
		if (createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			std::cerr << "\nVertex Shader Compile Time Error\n";
			glDeleteShader(vertexShaderID);
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
			std::cerr << "\nShader Error: Link Time Error:\n\n" <<
				infoLog << "\n\n";

			std::cin.get();
		}
		else
		{
			m_isCompiled = true;
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	ShaderProgram::~ShaderProgram()
	{
	}

	bool ShaderProgram::createShader(const std::string& sourse,
		const GLenum shaderType, GLuint& shaderID)
	{
		shaderID = glCreateShader(shaderType);
		const char* code = sourse.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			std::cerr << "Shader Error: Compile Time Error:\n\n" <<
				infoLog << "\n\n";

			std::cin.get();

			return false;
		}
		return true;
	}
}