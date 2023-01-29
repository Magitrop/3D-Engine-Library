#include "pch.h"
#include "Lightings.h"
#include "LightSourceComponent.h"

Matrix4x4 LightingsSingleton::GetLightProjectionMatrix()
{
	return lightProjection;
}

Matrix4x4 LightingsSingleton::GetLightViewMatrix()
{
	return lightView;
}

Matrix4x4 LightingsSingleton::GetLightSpaceMatrix()
{
	return lightSpaceMatrix;
}

Vector2 LightingsSingleton::GetShadowMapScale()
{
	return Vector2(SHADOW_WIDTH, SHADOW_HEIGHT);
}

unsigned int LightingsSingleton::GetShadowMapWidth()
{
	return SHADOW_WIDTH;
}

unsigned int LightingsSingleton::GetShadowMapHeight()
{
	return SHADOW_HEIGHT;
}
