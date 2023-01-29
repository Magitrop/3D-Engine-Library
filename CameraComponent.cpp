#pragma once
#include "pch.h"

#include "CameraComponent.h"
#include "Screen.h"

#include "TransformComponent.h"
#include "ObjectsManager.h"
#include "EventSystem.h"

#include <glm/gtx/projection.hpp>

void CameraComponent::RecalculateProjectionMatrix()
{
	if (perspective) // perspective
	{
		projectionMatrix =
			glm::perspective(
				glm::radians(fieldOfView),
				Screen->GetAspectRatio(),
				nearClipPlane,
				farClipPlane);
	}
	else // ortho
	{
		projectionMatrix =
			glm::ortho(
				-orthoScale * Screen->GetAspectRatio(), orthoScale * Screen->GetAspectRatio(),
				-orthoScale, orthoScale,
				nearClipPlane,
				farClipPlane);
	}
	projectionViewMatrix = projectionMatrix * viewMatrix;
	projectionViewMatrixInverted = glm::inverse(projectionMatrix * viewMatrix);
}
void CameraComponent::RecalculateViewMatrix()
{
	viewMatrix =
		glm::lookAt(
			gameObject->transform->GetPosition(),
			gameObject->transform->GetPosition() + gameObject->transform->GetForward(),
			Vectors::up
		);
	projectionViewMatrix = projectionMatrix * viewMatrix;
	projectionViewMatrixInverted = glm::inverse(projectionMatrix * viewMatrix);
}

void CameraComponent::SetAsPerspective(bool asPerspective)
{
	perspective = asPerspective;
}

void CameraComponent::SetFieldOfView(float fov)
{
	fieldOfView = fov;
	RecalculateProjectionMatrix();
}
void CameraComponent::SetClippingPlanes(float near, float far)
{
	nearClipPlane = near;
	farClipPlane = far;
	RecalculateProjectionMatrix();
}
const glm::Matrix4x4& CameraComponent::GetProjectionMatrix() const
{
	return projectionMatrix;
}
const glm::Matrix4x4& CameraComponent::GetViewMatrix() const
{
	return viewMatrix;
}
const glm::Matrix4x4& CameraComponent::GetProjectionViewMatrix() const
{
	return projectionViewMatrix;
}

const glm::Matrix4x4& CameraComponent::GetInvertedProjectionViewMatrix() const
{
	return projectionViewMatrixInverted;
}

Ray CameraComponent::ScreenPointToRay(Vector2 screenPoint) const
{
	Vector4 point = Vectors::AsVector4(screenPoint);
	Matrix4x4 invMat = GetInvertedProjectionViewMatrix();
	Vector4 near(2.f * point.x / Screen->GetWindowResolution().x - 1.f, 1.f - 2.f * point.y / Screen->GetWindowResolution().y, -1, 1.0);
	Vector4 far(2.f * point.x / Screen->GetWindowResolution().x - 1.f, 1.f - 2.f * point.y / Screen->GetWindowResolution().y, 1, 1.0);
	Vector4 nearResult = invMat * near;
	Vector4 farResult = invMat * far;
	nearResult /= nearResult.w;
	farResult /= farResult.w;
	return Ray(gameObject->transform->GetPosition(), glm::normalize(Vector3(farResult - nearResult)));
}

Vector3 CameraComponent::ScreenToWorldPoint(Vector2 screenPoint) const
{
	Vector4 point = Vectors::AsVector4(screenPoint);
	Matrix4x4 invMat = glm::inverse(GetProjectionViewMatrix());
	Vector4 near(2 * point.x / Screen->GetWindowResolution().x - 1.f, 1.f - 2 * point.y / Screen->GetWindowResolution().y, -1, 1.0);
	Vector4 far(2 * point.x / Screen->GetWindowResolution().x - 1.f, 1.f - 2 * point.y / Screen->GetWindowResolution().y, 1, 1.0);
	Vector4 nearResult = invMat * near;
	Vector4 farResult = invMat * far;
	nearResult /= nearResult.w;
	farResult /= farResult.w;
	return gameObject->transform->GetPosition() + glm::normalize(Vector3(farResult - nearResult));
}

void CameraComponent::OnComponentAdded()
{
	RecalculateProjectionMatrix();
	RecalculateViewMatrix();
}

void CameraComponent::OnUpdate()
{
	RecalculateProjectionMatrix();
	RecalculateViewMatrix();
}

void CameraComponent::SetOrthoScale(float scale)
{
	orthoScale = scale;
}

void CameraComponent::Serialize(Json& writeTo)
{
	writeTo[GetComponentName()]["fieldOfView"] = fieldOfView;
	writeTo[GetComponentName()]["nearClipPlane"] = nearClipPlane;
	writeTo[GetComponentName()]["farClipPlane"] = farClipPlane;
	writeTo[GetComponentName()]["perspective"] = perspective;
	writeTo[GetComponentName()]["orthoScale"] = orthoScale;
}

void CameraComponent::Deserialize(const Json& readFrom)
{
	 fieldOfView = readFrom[GetComponentName()]["fieldOfView"];
	 nearClipPlane = readFrom[GetComponentName()]["nearClipPlane"];
	 farClipPlane = readFrom[GetComponentName()]["farClipPlane"];
	 perspective = readFrom[GetComponentName()]["perspective"];
	 orthoScale = readFrom[GetComponentName()]["orthoScale"];
}

std::string CameraComponent::GetComponentName()
{
	return "Camera";
}