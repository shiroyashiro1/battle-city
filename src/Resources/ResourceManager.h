#pragma once
#include <string>
#include <memory>
#include <map>

namespace Renderer
{
	class ShaderProgram;
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

private:
	std::string getFileString(const std::string& relateveFilePath) const;

	/* Map for save our shaders */
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramMap;
	ShaderProgramMap m_shaderProgarams;

	std::string m_path;
};