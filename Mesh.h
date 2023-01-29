#pragma once
#include "ModelRendererComponent.h"
#include <vector>
#include <string>

enum class DrawMode
{
    Solid,
    Lines,
    Wireframe,
    Points
};

struct Vertex final 
{
    Vector3 Position = Vector3();
    Vector3 Normal = Vector3();
    Vector2 TexCoords = Vector2();
    
    Vertex(
        Vector3 pos = Vector3(), 
        Vector3 normal = Vector3(), 
        Vector3 texcords = Vector3()) 
        : Position(pos), Normal(normal), TexCoords(texcords) {}
};

struct Texture final 
{
    unsigned int id;
    std::string type;
};

class Shader;
class Mesh final 
{
private:
    friend int main();
    friend class ModelRendererComponent;
    friend class EngineEditor;
    unsigned int VAO, VBO, EBO;

    std::vector<Vector2> vertexTexturePositions;
    std::vector<Vector3> vertexNormals;
    std::vector<float> vertexPositions;

    DrawMode drawMode = DrawMode::Solid;

    void SetupMesh();
    void RecalculateVertexNormals();
    std::vector<float>& GetVertexPositions();
    std::vector<Vector3>& GetVertexNormals();
    std::vector<Vector2>& GetVertexTexturePositions();
    void Render(const Shader& shader);
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    float wireframeWidth = 1;

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, bool recalculateNormals = true);
    void SetDrawMode(DrawMode mode);
    //Mesh(std::vector<Vertex>& vertices, std::vector<Vector3>& triangles, std::vector<Texture>& textures);
};
