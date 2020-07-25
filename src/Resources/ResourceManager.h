#pragma once
#include <string>
#include <memory>
#include <map>

namespace Renderer
{
	class ShaderProgram;
	class Texture2D;
}

class ResourceManager
{
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	/* Prohibit copying class object */
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShader(
		const std::string& shaderName,
		const std::string& vertexPath, 
		const std::string& fragmentPath
	);
	std::shared_ptr<Renderer::ShaderProgram> getShader(
		const std::string& shaderName
	);

	std::shared_ptr <Renderer::Texture2D> loadTexture(
		const std::string& textureName, const std::string& texturePath);
	std::shared_ptr <Renderer::Texture2D> ResourceManager::getTexture(
		const std::string& textureName, const std::string& texturePath);

private:
	std::string getFileString(const std::string& relateveFilePath) const;

	/* Map for save our shaders */
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramMap;
	ShaderProgramMap m_shaderProgarams;

	/* Map for save our textures */
	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	TexturesMap m_textures;

	std::string m_path;
};