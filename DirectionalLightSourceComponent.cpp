#include "pch.h"

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "DirectionalLightSourceComponent.h"

#include "Screen.h"
#include "GameObject.h"
#include "ResourceManager.h"

#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include "Lightings.h"

void DirectionalLightSourceComponent::OnDestroy()
{
	// remove from light sources list
}

void DirectionalLightSourceComponent::OnComponentAdded()
{
	RecalculateDepthMap();

	if (glIsFramebuffer(depthMapFBO))
		glDeleteFramebuffers(1, &depthMapFBO);
	glGenFramebuffers(1, &depthMapFBO);

	if (glIsTexture(depthMap))
		glDeleteTextures(1, &depthMap);
	glGenTextures(1, &depthMap);

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		Lightings->SHADOW_WIDTH, Lightings->SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLightSourceComponent::RecalculateDepthMap()
{
	float near_plane = 0.1f, far_plane = 100.f;
	//lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, near_plane, far_plane);
	lightProjection = glm::perspective(glm::radians(90.0f), Screen->GetAspectRatio(), near_plane, far_plane);
	/*lightView =
		glm::rotate(Matrix4x4(1), glm::radians(60.f), Vectors::right) *
		glm::rotate(Matrix4x4(1), glm::radians(lightRot.y), Vectors::up) *
		glm::translate(Matrix4x4(1), -lightPos);*/
	lightView =
		glm::lookAt(
			gameObject->transform->GetPosition(),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}

Matrix4x4 DirectionalLightSourceComponent::GetLightProjectionMatrix()
{
	return lightProjection;
}

Matrix4x4 DirectionalLightSourceComponent::GetLightViewMatrix()
{
	return lightView;
}

Matrix4x4 DirectionalLightSourceComponent::GetLightSpaceMatrix()
{
	return lightSpaceMatrix;
}

unsigned int DirectionalLightSourceComponent::GetDepthMapID()
{
	return depthMap;
}

void DirectionalLightSourceComponent::Serialize(Json& writeTo)
{
	writeTo[GetComponentName()]["sample"] = 1;
}

void DirectionalLightSourceComponent::Deserialize(const Json& readFrom)
{
	
}

std::string DirectionalLightSourceComponent::GetComponentName()
{
	return "Directional Light";
}