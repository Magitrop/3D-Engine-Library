#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <memory>
#include <vector>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ObjectsManager.h"
#include "TransformComponent.h"
#include "RendererComponentBase.h"
#include "EventSystem.h"
#include "Model.h"

class Model;
class Shader;
enum class DrawMode;
class ModelRendererComponent final : public RendererComponentBase
{
	COMPONENT(ModelRendererComponent, RendererComponentBase)
protected:
	friend class EngineEditor;

	std::string preloadedModelName;
	const Model* model = nullptr;

	std::string preloadedShaderName;
	const Shader* currentShader = nullptr;

	virtual void Render() override;
	virtual void RenderDepth() override;
public:
	DrawMode drawMode;

	void SetModel(const Model* newModel);
	void SetModel(std::string modelName);	// uploaded models only
	void SetShader(const Shader* newShader);
	void SetShader(std::string shaderName);	// uploaded shaders only

	void Serialize(Json& writeTo) override;
	void Deserialize(const Json& readFrom) override;
	virtual std::string GetComponentName() override;
};