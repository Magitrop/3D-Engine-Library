#include "pch.h"
#include "ObjectsManager.h"

GameObject* ObjectsManagerSingleton::Instantiate()
{
	GameObject* obj = new GameObject();
	obj->gameObjectID = nextObjectID++;
	instantiatedObjects.push_back(obj);
	return obj;
}

GameObject* ObjectsManagerSingleton::Instantiate(const Vector3& position)
{
	GameObject* obj = new GameObject();
	obj->gameObjectID = nextObjectID++;
	obj->transform->SetPosition(position);
	instantiatedObjects.push_back(obj);
	return obj;
}

std::list<GameObject*>::iterator ObjectsManagerSingleton::DestroyObject(GameObject* toDestroy)
{
	auto it = std::find(instantiatedObjects.begin(), instantiatedObjects.end(), toDestroy);
	if (it != instantiatedObjects.end())
	{
		delete toDestroy;
		return instantiatedObjects.erase(it);
	}
	return instantiatedObjects.end();
}