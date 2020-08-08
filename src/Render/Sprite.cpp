#include "Sprite.h"

namespace Renderer
{
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string initialSubTexture,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float rotation) 
		: m_pTexture(std::move(pTexture)),
		m_pShaderProgram(std::move(pShaderProgram)),
		m_position(position),
		m_size(size),
		m_rotation(rotation)
	{
		const GLfloat vertexCoords[] =
		{
			// Two triangles in a row
			// This is our square sprite

		//   x    y
			0.f, 0.f,
		    0.f, 1.f,
			1.f, 1.f,

			1.f, 1.f,
			1.f, 0.f,
			0.f, 0.f
		};

		auto subTexture = pTexture->getSubTexture(std::move(initialSubTexture));

		const GLfloat textureCoords[] =
		{
			// Two triangles in a row
			// This is our triagle and triagle textures
			// together

		//   u    v
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.rightTopUV.y,

			subTexture.rightTopUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_vertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &m_textureCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Sprite::~Sprite()
	{
		glDeleteBuffers(1, &m_vertexCoordsVBO);
		glDeleteBuffers(1, &m_textureCoordsVBO);
		glDeleteBuffers(1, &m_VAO);
	}

	void Sprite::render() const
	{
		// Activated our shaders
		m_pShaderProgram->use();
		// Model matrix for rotation, translating and scaling
		glm::mat4 modelMatrix(1.f);

		// WE DO ALL IN REVERSE ORDER

		// (5) Translating our sprite there we want
		modelMatrix = glm::translate(modelMatrix, glm::vec3(m_position.x, m_position.y, 0.f));

		// (4) Translating our sprite to half against origin
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * m_size.x,
			0.5f * m_size.y, 0.f));

		// (3) Rotation for our sprite
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
		// (2) Translating our sprite to origin
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * m_size.x,
			-0.5f * m_size.y, 0.f));
		// (1) Scaling our sprite
		modelMatrix = glm::scale(modelMatrix, glm::vec3(m_size, 1.f));

		glBindVertexArray(m_VAO);
		m_pShaderProgram->setMatrix4("modelMatrix", modelMatrix);

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Sprite::setPosition(const glm::vec2& position)
	{
		m_position = position;
	}

	void Sprite::setSize(const glm::vec2& size)
	{
		m_size = size;
	}

	void Sprite::setRotation(const float rotation)
	{
		m_rotation = rotation;
	}
}