#pragma once
#include "Component.h"

class DirectionalLightSourceComponent final : public Component
{
	COMPONENT(DirectionalLightSourceComponent, Component)
private:
	friend class LightingController;
	friend class RenderingController;
	friend class EngineEditor;

	Matrix4x4 lightProjection;
	Matrix4x4 lightView;
	Matrix4x4 lightSpaceMatrix;

	unsigned int depthMapFBO;
	unsigned int depthMap;
protected:
	virtual void OnDestroy() override;
	virtual void OnComponentAdded() override;

	void RecalculateDepthMap();
public:
	virtual void Serialize(Json& writeTo) override;
	virtual void Deserialize(const Json& readFrom) override;
	virtual std::string GetComponentName() override;

	Matrix4x4 GetLightProjectionMatrix();
	Matrix4x4 GetLightViewMatrix();
	Matrix4x4 GetLightSpaceMatrix();
	unsigned int GetDepthMapID();
};
