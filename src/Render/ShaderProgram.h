#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Renderer 
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader,
			const std::string& fragmentShader);
		~ShaderProgram();

		bool isCompiled() const { return m_isCompiled; }
		bool use() const;

	private:
		bool createShader(const std::string& sourse, 
			const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}