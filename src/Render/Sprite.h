#pragma once

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace Renderer
{
	class Texture2D;

	class Sprite
	{
	public:
		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		Sprite(const std::shared_ptr<Texture2D> pTexture, 
			const std::string initialSubTexture,
			const std::shared_ptr<ShaderProgram> pShaderProgram,
			const glm::vec2& position = glm::vec2(0.f), 
			const glm::vec2& size = glm::vec2(1.f),
			const float relation = 0.f);
		
		~Sprite();

		void render() const;
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size);
		void setRotation(const float rotation);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
		glm::vec2 m_position;
		glm::vec2 m_size;
		float m_rotation;

		GLuint m_VAO;
		GLuint m_vertexCoordsVBO;
		GLuint m_textureCoordsVBO;

	};
}