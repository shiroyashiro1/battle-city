#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>

namespace Renderer 
{
	class ShaderProgram
	{
	public:
		// Can not do 
		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		// Can do
		ShaderProgram(const std::string& vertexShader,
			const std::string& fragmentShader);
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
		void setInt(const std::string& name, const GLint value);

		~ShaderProgram();

		bool isCompiled() const { return m_isCompiled; }
		void use() const;

	private:
		bool createShader(const std::string& sourse, 
			const GLenum shaderType, GLuint& shaderID);

		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}
/*
namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		bool isCompiled() const { return m_isCompiled; }
		void use() const;

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);

		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}*/