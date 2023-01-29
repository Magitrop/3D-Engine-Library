#pragma once
#include "pch.h"

#include "Component.h"

class LightingController;
class RendererComponentBase : public Component
{
	COMPONENT(RendererComponentBase, Component)
private:
	friend class LightingController;
	friend class RenderingController;
	friend class EngineEditor;
protected:
	virtual void OnDestroy() override;
	virtual void OnComponentAdded() override;

	virtual void Render() = 0;
	virtual void RenderDepth() {}
public:
	bool castShadows = true;
	bool receiveShadows = true;
	int renderQueueIndex = 0;

	virtual void Serialize(Json& writeTo) override = 0;
	virtual void Deserialize(const Json& readFrom) override = 0;
	virtual std::string GetComponentName() override = 0;
};