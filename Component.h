#pragma once
#include "pch.h"

#include <iostream>
#include <GLFW\glfw3.h>
#include "Vectors.h"
#include "JsonSerializable.h"

class GameObject;

#define COMPONENT(name, base) \
friend class GameObject; \
protected: \
explicit name(GameObject* gameObject) : base(gameObject) { OnComponentAdded(); std::cout << " " << GetComponentName() << std::endl; } \
virtual ~name() override { OnDestroy(); } \

class Component : public JsonSerializable
{
	friend class GameObject;
	friend class EngineEditor;
protected:
	explicit Component(GameObject* gameObject) : gameObject(gameObject) { OnComponentAdded(); }

	Component(Component& other) = delete;
	virtual Component& operator = (Component& other) final = delete;

	void operator delete(void* ptr)
	{
		::operator delete(ptr);
	}

	void operator delete[](void* ptr)
	{
		::operator delete[](ptr);
	}

	virtual ~Component();

	virtual void OnComponentAdded();
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate() {}
	virtual void OnPreRender() {}
	virtual void OnPostRender() {}
	virtual void OnMouseMove(GLFWwindow* window, double x, double y, Vector2 motion) {}
	virtual void OnMouseClick(GLFWwindow* window, int button, int action, int mods) {}
	virtual void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {}
	virtual void OnKeyboardButton(GLFWwindow* window, int key, int scancode, int action, int mode) {}
	virtual void OnWindowResize(GLFWwindow* window, int width, int height) {}

	unsigned long long componentID = ULLONG_MAX;
public:
	GameObject* const gameObject;

	virtual void Serialize(Json& writeTo) override = 0;
	virtual void Deserialize(const Json& readFrom) override = 0;
	virtual std::string GetComponentName() = 0;
	unsigned long long GetComponentID() const;
};

class CannotBeRemovedComponentAttribute {};
class MustBeUniqueComponentAttribute {};