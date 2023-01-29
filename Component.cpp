#include "pch.h"
#include "Component.h"
#include "ObjectsManager.h"
#include "EventSystem.h"

#define ATTACH_UPDATE \
EventSystem->AttachUpdateEventListener(GetComponentID(), \
[=]() { OnUpdate(); }); \

#define ATTACH_MOUSE_MOVE \
EventSystem->AttachMouseMoveEventListener(GetComponentID(), \
[=](GLFWwindow* window, double x, double y, Vector2 motion) \
	{ OnMouseMove(window, x, y, motion); }); \

#define ATTACH_MOUSE_BUTTON \
EventSystem->AttachMouseButtonEventListener(GetComponentID(), \
[=](GLFWwindow* window, int button, int action, int mods) \
	{ OnMouseClick(window, button, action, mods); }); \

#define ATTACH_MOUSE_WHEEL \
EventSystem->AttachMouseWheelEventListener(GetComponentID(), \
[=](GLFWwindow* window, double xoffset, double yoffset) \
	{ OnMouseWheel(window, xoffset, yoffset); }); \

#define ATTACH_KEYBOARD \
EventSystem->AttachKeyboardEventListener(GetComponentID(), \
[=](GLFWwindow* window, int key, int scancode, int action, int mode) \
	{ OnKeyboardButton(window, key, scancode, action, mode); }); \

#define ATTACH_WINDOW_RESIZE \
EventSystem->AttachWindowResizeEventListener(GetComponentID(), \
[=](GLFWwindow* window, int width, int height) \
	{ OnWindowResize(window, width, height); }); \

#define DETACH_UPDATE \
EventSystem->ReleaseUpdateEventListener(GetComponentID()); \

#define DETACH_MOUSE_MOVE \
EventSystem->ReleaseMouseMoveEventListener(GetComponentID()); \

#define DETACH_MOUSE_BUTTON \
EventSystem->ReleaseMouseButtonEventListener(GetComponentID()); \

#define DETACH_MOUSE_WHEEL \
EventSystem->ReleaseMouseWheelEventListener(GetComponentID()); \

#define DETACH_KEYBOARD \
EventSystem->ReleaseKeyboardEventListener(GetComponentID()); \

#define DETACH_WINDOW_RESIZE \
EventSystem->ReleaseWindowResizeEventListener(GetComponentID()); \

void Component::OnComponentAdded()
{
	if (componentID == ULLONG_MAX)
	{
		componentID = reinterpret_cast<unsigned long long>(this);//componentID = ObjectsManager->nextComponentID++;
		std::cout << gameObject->name << " : " << ObjectsManager->nextComponentID++ << " ";
	}
	ATTACH_UPDATE;
	ATTACH_MOUSE_WHEEL;
	ATTACH_MOUSE_MOVE;
	ATTACH_MOUSE_BUTTON;
	ATTACH_KEYBOARD;
	ATTACH_WINDOW_RESIZE;
}

unsigned long long Component::GetComponentID() const
{
	return componentID;
}

Component::~Component()
{
	if (EventSystem->InPlayMode())
		OnDestroy();
	DETACH_UPDATE;
	DETACH_MOUSE_WHEEL;
	DETACH_MOUSE_MOVE;
	DETACH_MOUSE_BUTTON;
	DETACH_KEYBOARD;
	DETACH_WINDOW_RESIZE;
}