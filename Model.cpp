#include "pch.h"
#include "Model.h"
#include "Mesh.h"
#include "Screen.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <functional>

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Model::Model(std::string path)
{
	LoadModel(path);
}

Model::~Model()
{
	for (auto& m : meshes)
		delete m;
}

Model::Model(const std::vector<Mesh*>& _meshes)
{
	meshes = _meshes;
}

void Model::SetDrawMode(DrawMode mode) const
{
	if (currentDrawMode == mode)
		return;
	currentDrawMode = mode;
	for (auto& mesh : meshes)
		mesh->SetDrawMode(mode);
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, 
		aiProcess_Triangulate | 
		aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs | 
		aiProcess_PreTransformVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	std::function<void(aiNode*, const aiScene*)> getVerticesCount = [&](aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			verticesCount += mesh->mNumVertices;
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			getVerticesCount(node->mChildren[i], scene);
		}
	};
	getVerticesCount(scene->mRootNode, scene);

	ProcessNode(scene->mRootNode, scene);

	aiQuaternion q;
	aiVector3D v;
	scene->mRootNode->mTransformation.DecomposeNoScaling(q, v);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, node));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const aiNode* node)
{
	aiVector3D s;
	aiVector3D r;
	aiVector3D p;
	node->mTransformation.Decompose(s, r, p);
	Vector3 nodePosition(p.x, p.y, p.z);
	Vector3 nodeScaling(s.x, s.y, s.z);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(
			{
				Vector3(
					mesh->mVertices[i].x + nodePosition.x,
					mesh->mVertices[i].y + nodePosition.y,
					mesh->mVertices[i].z + nodePosition.z),
				Vector3(
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z),
				mesh->mTextureCoords[0]
				? Vector3(
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y, 0)
				: Vector3(0)
			});
	}
	// indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// seems to work
		indices.reserve(indices.size() + face.mNumIndices);
		indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
		/*for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);*/
	}

	// material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = LoadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	loadedVerticesCount += vertices.size();
	std::string count = std::to_string(loadedVerticesCount) + "/" + std::to_string(verticesCount) + "\n";
	std::cout << count;

	return new Mesh(vertices, indices, textures, false);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;

		std::string s;
		for (int i = 0; i < str.length; i++)
			if (s.length() == 0 && str.data[i] == '\0') 
				continue;
			else 
				s += str.data[i];
		s = s.substr(s.find_last_of("/\\") + 1);
		std::string dir = directory.substr(0, directory.find_last_of('.'));
		texture.id = TextureFromFile(s + ".png", dir);
		std::cout << "texture id: " << texture.id << std::endl;
		texture.type = typeName;
		//texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}

unsigned int Model::TextureFromFile(std::string path, const std::string& directory)
{
	std::string filename = directory + "\\" + path;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename << "(error: " << stbi_failure_reason() << ")" << std::endl;
	}

	stbi_image_free(data);
	return textureID;
}