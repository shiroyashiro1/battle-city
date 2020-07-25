#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer
{
	class Texture2D
	{
	public:
		Texture2D(const GLuint width, const GLuint height, // width and height texture
			// every pixel of texture
			const unsigned char* data, 
			// 4 channels of color
			const unsigned int channels = 4, 
			// filter (anti-alasing)
			const GLenum filter = GL_NEAREST, 
			// sets the wrap parameter for texture
			const GLenum wrapMode = GL_CLAMP_TO_EDGE
		);
		~Texture2D();

		/* Prohibition */
		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		/* Move operator*/
		Texture2D& operator=(Texture2D&& texture2d);
		Texture2D(Texture2D&& texture2d);

		void bind() const;

	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width;
		unsigned int m_height;

	};
}