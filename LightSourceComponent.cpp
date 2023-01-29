#include "pch.h"

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "LightSourceComponent.h"
#include "Lightings.h"
#include "Screen.h"
#include "GameObject.h"

void LightSourceComponent::OnDestroy()
{
	Lightings->lightSources.erase(std::find(Lightings->lightSources.begin(), Lightings->lightSources.end(), this));
}

void LightSourceComponent::OnComponentAdded()
{
	Lightings->lightSources.push_back(this);

	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	// The texture we're going to render to
	glGenTextures(1, &depthMap);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, depthMap);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Lightings->SHADOW_WIDTH, Lightings->SHADOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Lightings->SHADOW_WIDTH, Lightings->SHADOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depthMap, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Failed to create framebuffer" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightSourceComponent::Serialize(Json& writeTo)
{
	writeTo[GetComponentName()]["specular"] = specular;
	writeTo[GetComponentName()]["intensity"] = intensity;
	writeTo[GetComponentName()]["[lightType]type"] = type;
	writeTo[GetComponentName()]["color"]["r"] = color.r;
	writeTo[GetComponentName()]["color"]["g"] = color.g;
	writeTo[GetComponentName()]["color"]["b"] = color.b;
}

void LightSourceComponent::Deserialize(const Json& readFrom)
{
	specular = readFrom[GetComponentName()]["specular"];
	intensity = readFrom[GetComponentName()]["intensity"];
	type = readFrom[GetComponentName()]["[lightType]type"];
	color = Color(
		readFrom[GetComponentName()]["color"]["r"],
		readFrom[GetComponentName()]["color"]["g"],
		readFrom[GetComponentName()]["color"]["b"],
		1);
}

std::string LightSourceComponent::GetComponentName()
{
    return "Light Source";
}

void LightSourceComponent::RecalculateDepthMap()
{
	float near_plane = 0.01f, far_plane = 100.f;
	//lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, near_plane, far_plane);
	lightProjection = 
		glm::perspective(
			glm::radians(45.f),
			Screen->GetAspectRatio(),
			near_plane,
			far_plane);
	lightView =
		glm::lookAt(
			gameObject->transform->GetPosition(),
			gameObject->transform->GetPosition() + gameObject->transform->GetForward(),
			Vectors::up
		);
	lightSpaceMatrix = lightProjection * lightView;
}

Matrix4x4 LightSourceComponent::GetLightProjectionMatrix()
{
	return lightProjection;
}

Matrix4x4 LightSourceComponent::GetLightViewMatrix()
{
	return lightView;
}

Matrix4x4 LightSourceComponent::GetLightSpaceMatrix()
{
	return lightSpaceMatrix;
}

unsigned int LightSourceComponent::GetDepthMapID()
{
	return depthMap;
}