#pragma once
#include "pch.h"

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "LightSourceComponent.h"
#include "ModelRendererComponent.h"
#include "Shader.h"
#include "Lightings.h"
#include "ResourceManager.h"

void ModelRendererComponent::Render()
{
	if (!model || !currentShader)
		return;

	currentShader->Use();

	currentShader->setMat4("view", EventSystem->GetMainCamera()->GetViewMatrix());
	currentShader->setMat4("proj", EventSystem->GetMainCamera()->GetProjectionMatrix());
	currentShader->setVec3("pos", gameObject->transform->GetPosition());

	currentShader->setMat4("MVP", gameObject->transform->GetMVPMatrix());
	currentShader->setMat4("projection", EventSystem->GetMainCamera()->GetProjectionMatrix());
	currentShader->setMat4("view", EventSystem->GetMainCamera()->GetViewMatrix());
	currentShader->setMat4("model", gameObject->transform->GetModelMatrix());

	for (int i = 0; i < MaxDirectionalLights; i++)
		currentShader->SetFloat("directionalLights[" + std::to_string(i) + "].intensity", 0);
	for (int i = 0; i < MaxPointLights; i++)
		currentShader->SetFloat("pointLights[" + std::to_string(i) + "].intensity", 0);

	int dirIndex = 0, pointIndex = 0;
	for (auto& l : Lightings->lightSources)
	{
		if (!l->gameObject->isActive)
			continue;
		switch (l->type)
		{
		case LightSourceType::Directional:
			currentShader->setVec3("directionalLights[" + std::to_string(dirIndex) + "].direction", -l->gameObject->transform->GetForward());
			currentShader->SetFloat("directionalLights[" + std::to_string(dirIndex) + "].intensity", l->intensity);
			currentShader->setVec4("directionalLights[" + std::to_string(dirIndex) + "].color", l->color);
			currentShader->SetBool("directionalLights[" + std::to_string(dirIndex) + "].useSpecular", l->specular);
			currentShader->setMat4("lightSpaceMatrix[" + std::to_string(dirIndex) + "]", l->GetLightSpaceMatrix());
			dirIndex++;
			break;
		case LightSourceType::Point:
			currentShader->setVec3("pointLights[" + std::to_string(pointIndex) + "].position", l->gameObject->transform->GetPosition());
			currentShader->SetFloat("pointLights[" + std::to_string(pointIndex) + "].intensity", l->intensity);
			currentShader->setVec4("pointLights[" + std::to_string(pointIndex) + "].color", l->color);
			currentShader->SetBool("pointLights[" + std::to_string(dirIndex) + "].useSpecular", l->specular);
			pointIndex++;
			break;
		}
		//currentShader->setVec3("lightPos[" + std::to_string(i) + "]", l->gameObject->transform->GetPosition());
		//currentShader->setMat4("lightSpaceMatrix[" + std::to_string(i) + "]", l->GetLightSpaceMatrix());
	}
	//currentShader->setVec3("lightPos", Lightings->lightPosition);
	//currentShader->setMat4("lightSpaceMatrix", Lightings->GetLightSpaceMatrix());
	currentShader->setVec3("viewPos", EventSystem->GetMainCamera()->gameObject->transform->GetPosition());
	currentShader->SetBool("receiveShadows", receiveShadows);

	model->SetDrawMode(drawMode);

	for (auto& mesh : model->meshes)
		mesh->Render(*currentShader);

	glUseProgram(0);
}

void ModelRendererComponent::RenderDepth()
{
	if (model == nullptr || !castShadows)
		return;

	for (auto& mesh : model->meshes)
	{
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, &mesh->indices[0]);
		glBindVertexArray(0);
	}
}

void ModelRendererComponent::SetModel(const Model* newModel)
{
	model = newModel;
}

void ModelRendererComponent::SetModel(std::string modelName)
{
	if (!ResourceManager->HasModel(modelName)) 
		return;
	model = ResourceManager->GetModel(modelName);
	preloadedModelName = modelName;
}

void ModelRendererComponent::SetShader(const Shader* newShader)
{
	currentShader = newShader;
}

void ModelRendererComponent::SetShader(std::string shaderName)
{
	if (!ResourceManager->HasShader(shaderName))
		return;
	currentShader = ResourceManager->GetShader(shaderName);
	preloadedShaderName = shaderName;
}

void ModelRendererComponent::Serialize(Json& writeTo)
{
	RendererComponentBase::Serialize(writeTo);
	writeTo[GetComponentName()]["[model]model"] = preloadedModelName;
	writeTo[GetComponentName()]["[shader]shader"] = preloadedShaderName;
	writeTo[GetComponentName()]["drawMode"] = drawMode;
}

void ModelRendererComponent::Deserialize(const Json& readFrom)
{
	RendererComponentBase::Deserialize(readFrom);
	SetModel(preloadedModelName = readFrom[GetComponentName()]["[model]model"]);
	SetShader(preloadedShaderName = readFrom[GetComponentName()]["[shader]shader"]);
	drawMode = readFrom[GetComponentName()]["drawMode"];
}

std::string ModelRendererComponent::GetComponentName()
{
	return "Model Renderer";
}