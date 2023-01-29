#include "pch.h"
#include "ResourceManager.h"
#include <stdexcept>

void ResourceManagerSingleton::RemoveShader(std::string shaderName)
{
    if (loadedShaders.find(shaderName) != loadedShaders.end())
    {
        delete loadedShaders[shaderName];
        loadedModels.erase(shaderName);
    }
    else
    {
        std::cout << "There is no loaded shader with name \"" << shaderName << "\"." << std::endl;
    }
}

void ResourceManagerSingleton::RemoveModel(std::string modelName)
{
    if (loadedModels.find(modelName) != loadedModels.end())
    {
        delete loadedModels[modelName];
        loadedModels.erase(modelName);
    }
    else
    {
        std::cout << "There is no loaded model with name \"" << modelName << "\"." << std::endl;
    }
}

void ResourceManagerSingleton::ClearResources()
{
    for (auto& s : loadedShaders)
        delete s.second;
    loadedShaders.clear();

    for (auto& m : loadedModels)
        delete m.second;
    loadedModels.clear();
}

void ResourceManagerSingleton::UploadShader(const Shader* shaderToLoad, std::string shaderName)
{
    if (loadedShaders.find(shaderName) != loadedShaders.end())
    {
        std::cout << "Cannot load shader with name \"" << shaderName << "\" as it already exists." << std::endl;
        return;
        //throw std::invalid_argument("Cannot load shader with name \"" + shaderName + "\" as it already exists.");
    }
    loadedShaders[shaderName] = shaderToLoad;
}
const Shader* ResourceManagerSingleton::GetShader(std::string shaderName)
{
    if (loadedShaders.find(shaderName) != loadedShaders.end())
        return loadedShaders[shaderName];
    else
    {
        std::cout << "There is no loaded shader with name \"" << shaderName << "\"." << std::endl;
        return nullptr;
    }
}

bool ResourceManagerSingleton::HasShader(std::string shaderName)
{
    return loadedShaders.find(shaderName) != loadedShaders.end();
}

void ResourceManagerSingleton::UploadModel(const Model* modelToLoad, std::string modelName)
{
    if (loadedModels.find(modelName) != loadedModels.end())
    {
        std::cout << "Cannot load model with name \"" << modelName << "\" as it already exists." << std::endl;
        return;
        //throw std::invalid_argument("Cannot load model with name \"" + modelName + "\" as it already exists.");
    }
    loadedModels[modelName] = modelToLoad;
}
const Model* ResourceManagerSingleton::GetModel(std::string modelName)
{
    if (loadedModels.find(modelName) != loadedModels.end())
        return loadedModels[modelName];
    else 
    {
        std::cout << "There is no loaded model with name \"" << modelName << "\"." << std::endl;
        return nullptr;
    }
}

bool ResourceManagerSingleton::HasModel(std::string modelName)
{
    return loadedModels.find(modelName) != loadedModels.end();
}
