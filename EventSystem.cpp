#pragma once
#include "pch.h"

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "EventSystem.h"
#include "GameObject.h"

const Vector2& EventSystemSingleton::GetMousePosition()
{
	return mousePos;
}
const Vector2& EventSystemSingleton::GetMouseMotion()
{
	return mouseMotion;
}
const Vector2& EventSystemSingleton::GetMouseWheelOffset()
{
	return mouseWheelOffset;
}
const bool EventSystemSingleton::GetKey(int keyCode)
{
	return keys[keyCode];
}
const bool EventSystemSingleton::GetKeyDown(int keyCode)
{
	return !keysLastFrame[keyCode] && keys[keyCode];
}
const bool EventSystemSingleton::GetKeyUp(int keyCode)
{
	return keysLastFrame[keyCode] && !keys[keyCode];
}
const bool EventSystemSingleton::GetMouseButton(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT: return leftMouseButton;
	case GLFW_MOUSE_BUTTON_RIGHT: return rightMouseButton;
	default: return false;
	}
}
const bool EventSystemSingleton::GetMouseButtonDown(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT: return !leftMouseButtonLastFrame && leftMouseButton;
	case GLFW_MOUSE_BUTTON_RIGHT: return !rightMouseButtonLastFrame && rightMouseButton;
	default: return false;
	}
}

const bool EventSystemSingleton::GetMouseButtonUp(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT: return leftMouseButtonLastFrame && !leftMouseButton;
	case GLFW_MOUSE_BUTTON_RIGHT: return rightMouseButtonLastFrame && !rightMouseButton;
	default: return false;
	}
}

const bool EventSystemSingleton::InPlayMode()
{
	return inPlayMode;
}

CameraComponent* const EventSystemSingleton::GetMainCamera()
{
	return mainCamera;
}

// Update event
void EventSystemSingleton::AttachUpdateEventListener(long long listenerID, std::function<void()> listener)
{
	onUpdate[listenerID] = listener;
}
// Mouse move event
void EventSystemSingleton::AttachMouseMoveEventListener(long long listenerID, std::function<void(GLFWwindow*, double, double, Vector2)> listener)
{
	onMouseMoveEvent[listenerID] = listener;
}
// Mouse button event
void EventSystemSingleton::AttachMouseButtonEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int, int)> listener)
{
	onMouseButtonEvent[listenerID] = listener;
}
// Mouse wheel event
void EventSystemSingleton::AttachMouseWheelEventListener(long long listenerID, std::function<void(GLFWwindow*, double, double)> listener)
{
	onMouseWheelEvent[listenerID] = listener;
}
// Keyboard event
void EventSystemSingleton::AttachKeyboardEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int, int, int)> listener)
{
	onKeyboardEvent[listenerID] = listener;
}
// Window resize event
void EventSystemSingleton::AttachWindowResizeEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int)> listener)
{
	onWindowResizeEvent[listenerID] = listener;
}

// Update event
void EventSystemSingleton::ReleaseUpdateEventListener(long long listenerID)
{
	onUpdate.erase(listenerID);
}
// Mouse move event
void EventSystemSingleton::ReleaseMouseMoveEventListener(long long listenerID)
{
	onMouseMoveEvent.erase(listenerID);
}
// Mouse button event
void EventSystemSingleton::ReleaseMouseButtonEventListener(long long listenerID)
{
	onMouseButtonEvent.erase(listenerID);
}
// Mouse wheel event
void EventSystemSingleton::ReleaseMouseWheelEventListener(long long listenerID)
{
	onMouseWheelEvent.erase(listenerID);
}
// Keyboard event
void EventSystemSingleton::ReleaseKeyboardEventListener(long long listenerID)
{
	onKeyboardEvent.erase(listenerID);
}
// Window resize event
void EventSystemSingleton::ReleaseWindowResizeEventListener(long long listenerID)
{
	onWindowResizeEvent.erase(listenerID);
}