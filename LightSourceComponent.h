#pragma once
#include "Component.h"

enum class LightSourceType
{
	Directional,
	Point
};

class LightSourceComponent final : public Component
{
	COMPONENT(LightSourceComponent, Component)
private:
	friend class LightingController;
	friend class RenderingController;
	friend class EngineEditor;
	friend int main();
protected:
	virtual void OnDestroy() override;
	virtual void OnComponentAdded() override;

	void RecalculateDepthMap();

	Matrix4x4 lightProjection;
	Matrix4x4 lightView;
	Matrix4x4 lightSpaceMatrix;

	unsigned int depthMapFBO;
	unsigned int depthMap;
public:
	LightSourceType type = LightSourceType::Directional;
	float intensity = 1;
	bool specular;
	Color color = Color(1, 1, 1, 1);

	virtual void Serialize(Json& writeTo) override;
	virtual void Deserialize(const Json& readFrom) override;
	virtual std::string GetComponentName() override;

	Matrix4x4 GetLightProjectionMatrix();
	Matrix4x4 GetLightViewMatrix();
	Matrix4x4 GetLightSpaceMatrix();
	unsigned int GetDepthMapID();
};