#pragma once
#include <glm/glm.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Vectors.h"
#include "Singleton.h"

#define Lightings SingletonManager::Instance()->Get<LightingsSingleton>()
constexpr auto MaxDirectionalLights = 8;
constexpr auto MaxPointLights = 16;

class LightingController;
class LightSourceComponent;
class LightingsSingleton final : SingletonBase
{
	SINGLETON(LightingsSingleton);
private:
	friend class LightingController;
	friend class LightSourceComponent;
	friend class DirectionalLightSourceComponent;

	Matrix4x4 lightProjection;
	Matrix4x4 lightView;
	Matrix4x4 lightSpaceMatrix;
	unsigned int SHADOW_WIDTH;
	unsigned int SHADOW_HEIGHT;
public:
	Vector3 lightPosition;
	std::vector<LightSourceComponent*> lightSources;

	Matrix4x4 GetLightProjectionMatrix();
	Matrix4x4 GetLightViewMatrix();
	Matrix4x4 GetLightSpaceMatrix();

	Vector2 GetShadowMapScale();
	unsigned int GetShadowMapWidth();
	unsigned int GetShadowMapHeight();
};