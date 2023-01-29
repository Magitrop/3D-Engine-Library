#pragma once
#include "Component.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>

#include <memory>
#include <vector>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Ray.h"

class CameraComponent final : public Component,
	MustBeUniqueComponentAttribute
{
	COMPONENT(CameraComponent, Component)
private:
	friend class EngineEditor;
	friend struct EditorCamera;
	friend int main();
protected:
	glm::Matrix4x4 projectionMatrix;
	glm::Matrix4x4 viewMatrix;
	glm::Matrix4x4 projectionViewMatrix;
	glm::Matrix4x4 projectionViewMatrixInverted;

	float orthoScale = 1;
	float fieldOfView = 45.f;
	float nearClipPlane = 0.01f;
	float farClipPlane = 100.f;
	// 0 - ortho
	// 1 - perspective
	bool perspective;

	void RecalculateProjectionMatrix();
	void RecalculateViewMatrix();

	virtual void OnComponentAdded() override;
	virtual void OnUpdate() override;
public:
	void SetOrthoScale(float scale);
	// 0 - ortho
	// 1 - perspective
	void SetAsPerspective(bool asPerspective);
	void SetFieldOfView(float fov);
	void SetClippingPlanes(float near, float far);
	const glm::Matrix4x4& GetProjectionMatrix() const;
	const glm::Matrix4x4& GetViewMatrix() const;
	const glm::Matrix4x4& GetProjectionViewMatrix() const;
	const glm::Matrix4x4& GetInvertedProjectionViewMatrix() const;
	Vector3 ScreenToWorldPoint(Vector2 screenPoint) const;
	Ray ScreenPointToRay(Vector2 screenPoint) const;

	void Serialize(Json& writeTo) override;
	void Deserialize(const Json& readFrom) override;
	virtual std::string GetComponentName() override;
};