#pragma once

#include <map>
#include <vector>
#include <list>
#include <functional>

#include "CameraComponent.h"
#include "Singleton.h"

#define EventSystem SingletonManager::Instance()->Get<EventSystemSingleton>()

class EventsControllerSingleton;
class EventSystemSingleton final : SingletonBase
{
	SINGLETON(EventSystemSingleton);
private:
	friend class EngineEditor;
	friend class EventsControllerSingleton;
	friend class Component;

	bool inPlayMode = false;

	std::map<unsigned long long, std::function<void()>> onUpdate;
	std::map<unsigned long long, std::function<void(GLFWwindow*, double, double, Vector2)>> onMouseMoveEvent;
	std::map<unsigned long long, std::function<void(GLFWwindow*, int, int, int)>> onMouseButtonEvent;
	std::map<unsigned long long, std::function<void(GLFWwindow*, double, double)>> onMouseWheelEvent;
	std::map<unsigned long long, std::function<void(GLFWwindow*, int, int, int, int)>> onKeyboardEvent;
	std::map<unsigned long long, std::function<void(GLFWwindow*, int, int)>> onWindowResizeEvent;

	std::vector<std::function<void()>> onUpdateEditor;
	std::vector<std::function<void(GLFWwindow*, double, double, Vector2)>> onMouseMoveEventEditor;
	std::vector<std::function<void(GLFWwindow*, int, int, int)>> onMouseButtonEventEditor;
	std::vector<std::function<void(GLFWwindow*, double, double)>> onMouseWheelEventEditor;
	std::vector<std::function<void(GLFWwindow*, int, int, int, int)>> onKeyboardEventEditor;
	std::vector<std::function<void(GLFWwindow*, int, int)>> onWindowResizeEventEditor;

	Vector2 mousePos;
	Vector2 prevMousePos;
	Vector2 mouseMotion;

	Vector2 mouseWheelOffset;

	bool leftMouseButtonLastFrame;
	bool rightMouseButtonLastFrame;
	bool leftMouseButton;
	bool rightMouseButton;

	bool keysLastFrame[GLFW_KEY_LAST];
	bool keys[GLFW_KEY_LAST];

	CameraComponent* mainCamera;

	// Update event
	void AttachUpdateEventListener(long long listenerID, std::function<void()> listener);
	// Mouse move event
	void AttachMouseMoveEventListener(long long listenerID, std::function<void(GLFWwindow*, double, double, Vector2)> listener);
	// Mouse button event
	void AttachMouseButtonEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int, int)> listener);
	// Mouse wheel event
	void AttachMouseWheelEventListener(long long listenerID, std::function<void(GLFWwindow*, double, double)> listener);
	// Keyboard event
	void AttachKeyboardEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int, int, int)> listener);
	// Window resize event
	void AttachWindowResizeEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int)> listener);

	// Update event
	void ReleaseUpdateEventListener(long long listenerID);
	// Mouse move event
	void ReleaseMouseMoveEventListener(long long listenerID);
	// Mouse button event
	void ReleaseMouseButtonEventListener(long long listenerID);
	// Mouse wheel event
	void ReleaseMouseWheelEventListener(long long listenerID);
	// Keyboard event
	void ReleaseKeyboardEventListener(long long listenerID);
	// Window resize event
	void ReleaseWindowResizeEventListener(long long listenerID);
public:
	const Vector2& GetMousePosition();
	const Vector2& GetMouseMotion();
	const Vector2& GetMouseWheelOffset();
	const bool GetKey(int keyCode);
	const bool GetKeyDown(int keyCode);
	const bool GetKeyUp(int keyCode);
	const bool GetMouseButton(int button);
	const bool GetMouseButtonDown(int button);
	const bool GetMouseButtonUp(int button);
	const bool InPlayMode();
	CameraComponent* const GetMainCamera();
};