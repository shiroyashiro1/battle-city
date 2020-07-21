#include "ResourceManager.h"
#include "../Render/ShaderProgram.h"

#include <sstream>
#include <fstream>

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
		std::cerr << "\nNo Vertex Shader!\n";
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "\nNo Fragment Shader!\n";
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderProgarams.emplace(shaderName, 
		std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;

	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "\nCan't load shader progtam:\n"
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

	std::cerr << "\nCan't find the shader program: " << shaderName << std::endl;
	return nullptr;
}