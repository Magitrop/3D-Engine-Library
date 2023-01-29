#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include "Mesh.h"

class Mesh;
struct Texture;
enum class DrawMode;
class Model
{
private:
    friend int main();
    friend class ModelRendererComponent;
    friend class EngineEditor;

    mutable DrawMode currentDrawMode;

    size_t loadedVerticesCount = 0;
    size_t verticesCount = 0;

    std::vector<Mesh*> meshes;

    std::string directory;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const aiNode* node);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(std::string path, const std::string& directory);
public:
    ~Model();
    Model(std::string path);
    Model(const std::vector<Mesh*>& _meshes = {});
    void SetDrawMode(DrawMode mode) const;
};