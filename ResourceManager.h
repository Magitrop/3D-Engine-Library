#pragma once
#include "pch.h"
#include "Singleton.h"
#include "Shader.h"
#include "Model.h"

#include <string>
#include <map>

#define ResourceManager SingletonManager::Instance()->Get<ResourceManagerSingleton>()

class ResourceManagerSingleton final : SingletonBase
{
	SINGLETON(ResourceManagerSingleton);
private:
	friend class EngineEditor;

	std::map<std::string, const Shader*> loadedShaders;
	std::map<std::string, const Model*> loadedModels;

	void RemoveShader(std::string shaderName);
	void RemoveModel(std::string modelName);
	void ClearResources();
public:
	void UploadShader(const Shader* shaderToLoad, std::string shaderName);
	const Shader* GetShader(std::string shaderName);
	bool HasShader(std::string shaderName);

	void UploadModel(const Model* modelToLoad, std::string modelName);
	const Model* GetModel(std::string modelName);
	bool HasModel(std::string modelName);
};