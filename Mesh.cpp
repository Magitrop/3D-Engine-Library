#include "pch.h"
#include <glad/glad.h>
#include "Shader.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, bool recalculateNormals)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	if (recalculateNormals)
		RecalculateVertexNormals();

	GetVertexNormals();
	GetVertexPositions();
	GetVertexTexturePositions();

	SetupMesh();
}

void Mesh::SetDrawMode(DrawMode mode)
{
	drawMode = mode;
}

void Mesh::RecalculateVertexNormals()
{
	std::vector<Vector3> faceNormals;
	for (int i = 0; i < indices.size(); i += 3)
	{
		Vector3 a
		(
			vertices[indices[i]].Position.x,
			vertices[indices[i]].Position.y,
			vertices[indices[i]].Position.z
		);
		Vector3 b
		(
			vertices[indices[i + 1]].Position.x,
			vertices[indices[i + 1]].Position.y,
			vertices[indices[i + 1]].Position.z
		);
		Vector3 c
		(
			vertices[indices[i + 2]].Position.x,
			vertices[indices[i + 2]].Position.y,
			vertices[indices[i + 2]].Position.z
		);
		Vector3 n = glm::normalize(glm::cross(b - a, c - a));
		faceNormals.push_back(n);
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		std::vector<int> trianglesUsingThisVertex;
		for (int j = 0; j < indices.size(); j += 3)
			for (int k = 0; k < 3; k++)
				// if triangle contains this vertex
				if (i == indices[j + k])
				{
					trianglesUsingThisVertex.push_back(j / 3);
					break;
				}
		Vector3 n = Vector3();
		for (int j = 0; j < trianglesUsingThisVertex.size(); j++)
		{
			n += faceNormals[trianglesUsingThisVertex[j]];
		}
		vertices[i].Normal = glm::normalize(n);
	}
}

void Mesh::Render(const Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	shader.SetInt("hasTextures", textures.size() > 0);
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i + 2);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		shader.SetInt((name + number).c_str(), i + 2);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	switch (drawMode)
	{
	case DrawMode::Solid:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
		break;
	case DrawMode::Wireframe:
		//glLineWidth(wireframeWidth);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
		break;
	case DrawMode::Points:
		//glLineWidth(wireframeWidth);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
		break;
	case DrawMode::Lines:
		//glLineWidth(wireframeWidth);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
		break;
	}

	glBindVertexArray(0);
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i + 2);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		glBindTexture(GL_TEXTURE_2D, 0);
		shader.SetInt((name + number).c_str(), 0);
	}
	glActiveTexture(GL_TEXTURE0);
}

std::vector<float>& Mesh::GetVertexPositions()
{
	vertexPositions.resize(vertices.size() * 3);

	size_t i = 0;
	for (auto& n : vertices)
	{
		vertexPositions[i++] = n.Position.x;
		vertexPositions[i++] = n.Position.y;
		vertexPositions[i++] = n.Position.z;
	}
	return vertexPositions;
}
std::vector<Vector3>& Mesh::GetVertexNormals()
{
	vertexNormals.resize(vertices.size());

	size_t i = 0;
	for (auto& n : vertices)
		vertexNormals[i++] = n.Normal;

	return vertexNormals;
}
std::vector<Vector2>& Mesh::GetVertexTexturePositions()
{
	vertexTexturePositions.resize(vertices.size());

	size_t i = 0;
	for (auto& n : vertices)
		vertexTexturePositions[i++] = n.TexCoords;

	return vertexTexturePositions;
}

void Mesh::SetupMesh()
{
	unsigned int vertexBuffer = 0;
	unsigned int trianglesBuffer = 0;
	unsigned int normalBuffer = 0;
	unsigned int texPosBuffer = 0;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// 1
	if (glIsBuffer(vertexBuffer))
		glDeleteBuffers(1, &vertexBuffer);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertexPositions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer
	(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 2
	if (glIsBuffer(trianglesBuffer))
		glDeleteBuffers(1, &trianglesBuffer);
	glGenBuffers(1, &trianglesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trianglesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// 3
	if (glIsBuffer(normalBuffer))
		glDeleteBuffers(1, &normalBuffer);
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertexNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer
	(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	// 4
	if (glIsBuffer(texPosBuffer))
		glDeleteBuffers(1, &texPosBuffer);
	glGenBuffers(1, &texPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexTexturePositions.size() * sizeof(Vector2), &vertexTexturePositions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer
	(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
	//    &indices[0], GL_STATIC_DRAW);

	//RecalculateVertexNormals();

	//// vertex positions
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//// vertex normals
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//// vertex texture coords
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	//glBindVertexArray(0);
}