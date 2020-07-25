#include "ResourceManager.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"

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
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "\nFILE ResourceManager.cpp | No Fragment Shader!\n";
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
	const std::string& textureName, const std::string& texturePath)
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
	return nullptr;
}