#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Singleton.h"

using glm::Vector2;
using glm::Vector3;
using glm::Vector4;

#define Screen SingletonManager::Instance()->Get<ScreenSingleton>()

class InitializationHandlerSingleton;
class ScreenSingleton final : SingletonBase
{
	SINGLETON(ScreenSingleton);
private:
	friend class InitializationHandlerSingleton;
	friend class EventsControllerSingleton;

	Vector2 windowSize = Vector2(1600, 800);
	GLFWwindow* window;
public:
	const Vector2& GetWindowResolution();
	float GetAspectRatio();

	void SetCursorVisibility(bool visible);
};