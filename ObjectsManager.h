#pragma once

#include <list>
#include <algorithm>

#include "GameObject.h"
#include "Singleton.h"

#define ObjectsManager SingletonManager::Instance()->Get<ObjectsManagerSingleton>()

class RendererComponentBase;
class ObjectsManagerSingleton final : SingletonBase
{
	SINGLETON(ObjectsManagerSingleton);
private:
	friend class EngineEditor;
	friend class RendererComponentBase;
	friend class RenderingController;
	friend class LightingController;
	friend class Component;
	friend class GameObject;

 	std::list<GameObject*> instantiatedObjects;
 	std::list<RendererComponentBase*> renderQueue;

 	unsigned long long nextObjectID = 0;
	unsigned long long nextComponentID = 0;
public:
	template<class T> T* Instantiate()
	{
		static_assert(std::is_base_of<Component, T>::value, "Only GameObject or Component may be instantiated.");

		return Instantiate()->AddComponent<T>();
	}
	template<class T> T* Instantiate(const Vector3& position)
	{
		static_assert(std::is_base_of<Component, T>::value, "Only GameObject or Component may be instantiated.");

		return Instantiate(position)->AddComponent<T>();
	}

 	GameObject* Instantiate();
 	GameObject* Instantiate(const Vector3& position);

	// returns iterator which follows the erased one
	std::list<GameObject*>::iterator DestroyObject(GameObject* toDestroy);

	// finds the first active object with component
	template<class T, class =
		typename std::enable_if<
		std::is_base_of<Component, T>::value>::type>
	 	T* FindObjectOfType()
	{
		T* comp;
		for (auto it = instantiatedObjects.begin(); it != instantiatedObjects.end(); it++)
			if ((*it)->isActive && (comp = (*it)->GetComponent<T>()))
				return comp;
		return nullptr;
	}
};