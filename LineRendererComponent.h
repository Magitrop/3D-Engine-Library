#pragma once
#include "pch.h"

#include "RendererComponentBase.h"
#include <vector>

class LineRendererComponent final : public RendererComponentBase
{
	COMPONENT(LineRendererComponent, RendererComponentBase)
public:
	float lineWidth = 1.0f;
	bool connectLines = true;
	Color color = Color(1, 1, 1, 1);

	void SetPoints(std::vector<Vector3>& points);
	void Serialize(Json& writeTo) override;
	void Deserialize(const Json& readFrom) override;
	virtual std::string GetComponentName() override;
protected:
	virtual void Render() override;
	virtual void OnComponentAdded() override;

	std::vector<unsigned int> indices;
	std::vector<Vector3> points;
	unsigned int VAO;
	unsigned int vertexBuffer = 0;
	unsigned int trianglesBuffer = 0;
};