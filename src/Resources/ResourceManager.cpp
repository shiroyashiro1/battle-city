#include "ResourceManager.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite.h"

#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);

	if (!f.is_open())
	{
		std::cerr << "\nFILE ResourceManager.cpp | Error getFileString : ifstream file can not open!\n" << 
			"Failed to open file : " << relativeFilePath << std::endl;

		std::cin.get();

		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr <Renderer::ShaderProgram> ResourceManager::loadShader(
	const std::string& shaderName, const std::string& vertexPath,
	const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		std::cerr << "\nFILE ResourceManager.cpp | No Vertex Shader!\n";
		std::cin.get();
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "\nFILE ResourceManager.cpp | No Fragment Shader!\n";
		std::cin.get();
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderProgarams.emplace(shaderName, 
		std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;

	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "\nFILE ResourceManager.cpp | Can't load shader progtam:\n"
		<< "Vertex: " << vertexPath
		<< "\nFragment: " << fragmentPath << std::endl;

	std::cin.get();

	return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShader(const std::string& shaderName)
{
	ShaderProgramMap::const_iterator iter = m_shaderProgarams.find(shaderName);
	if (iter != m_shaderProgarams.end())
	{
		return iter->second;
	}

	std::cerr << "\nFILE ResourceManager.cpp | Can't find the shader program: " << 
		shaderName << std::endl;

	std::cin.get();

	return nullptr;
}

std::shared_ptr <Renderer::Texture2D> ResourceManager::loadTexture(
	const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), 
		&width, &height, &channels, 0);

	if (!pixels)
	{
		std::cerr << "\nFILE ResourceManager.cpp | Can't load the texture : " << textureName << std::endl;
		std::cin.get();
		return nullptr;
	}

	/*std::cout << "\n1\n";
	std::cin.get();*/

	std::shared_ptr <Renderer::Texture2D> newTexture = m_textures.emplace(
		textureName, 
		std::make_shared <Renderer::Texture2D>(width, height, pixels, channels)
	).first->second;

	stbi_image_free(pixels);
	return newTexture;
}

std::shared_ptr <Renderer::Texture2D> ResourceManager::getTexture(
	const std::string& textureName)
{
	/*std::cout << "\n2\n";
	std::cin.get();*/
	TexturesMap::const_iterator iter = m_textures.find(textureName);
	if (iter != m_textures.end())
	{
		return iter->second;
	}
	/*std::cout << "\n3\n";
	std::cin.get();*/
	std::cerr << "\nFILE ResourceManager.cpp | Can't find the texture: " <<
		textureName << std::endl;

	std::cin.get();

	return nullptr;
}

std::shared_ptr <Renderer::Sprite> ResourceManager::loadSprite(
	const std::string& spriteName,
	const std::string& textureName,
	const std::string& shaderName,
	const unsigned int spriteWidth, const unsigned int spriteHeight,
	const std::string& subTextureName)
{
	auto pShader = getShader(shaderName);
	if (!pShader)
	{
		std::cerr << "\nFILE ResourceManager.cpp | Can't find the shader program: " <<
			shaderName << " for the sprite" << spriteName << std::endl;
		std::cin.get();
	}

	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "\nFILE ResourceManager.cpp | Can't find the texture: " <<
			textureName << " for the sprite" << spriteName << std::endl;
		std::cin.get();
	}

	std::shared_ptr <Renderer::Sprite> newSprite = m_sprites.emplace(textureName,
		std::make_shared <Renderer::Sprite>(pTexture, subTextureName, pShader, 
		glm::vec2(0.f, 0.f),
		glm::vec2(spriteWidth, spriteHeight)
		)
	).first->second;

	return newSprite;
}

std::shared_ptr <Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator iter = m_sprites.find(spriteName);
	if (iter != m_sprites.end())
	{
		return iter->second;
	}
	/*std::cout << "\n3\n";
	std::cin.get();*/
	std::cerr << "\nFILE ResourceManager.cpp | Can't find the sprite: " <<
		spriteName << std::endl;

	std::cin.get();

	return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTextureAtlas(const std::string textureName,
	const std::string texturePath,
	const std::vector<std::string> subTextures,
	const unsigned int subTextureWidth,
	const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
	if (pTexture)
	{
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textureHeight = pTexture->height();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;
		for (const auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);
			pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}
	}
	return pTexture;
}