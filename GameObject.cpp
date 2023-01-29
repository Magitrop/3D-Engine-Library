#include "pch.h"
#include "GameObject.h"
#include "TransformComponent.h"

// every game object must contain irremovable transform component
GameObject::GameObject() : name("New Object"), transform(AddComponent<TransformComponent>()) {}
GameObject::~GameObject()
{
    for (const auto& [componentName, component] : components)
    {
        delete component;
    }
    components.clear();
}

unsigned long long GameObject::GetGameObjectID() const
{
    return gameObjectID;
}

Json GameObject::Serialize() const
{
    Json result;
    for (auto it = components.begin(); it != components.end(); it++)
    {
        Json comp;
        it->second->Serialize(comp);
        comp["id"] = it->second->componentID;
        result[name].push_back(comp);
    }
    return result;
}

void GameObject::RemoveComponent(std::string componentName)
{
    for (auto& comp : components)
    {
        if (comp.second->GetComponentName() == componentName &&
            componentName != "Transform")
        {
            delete comp.second;
            components.erase(comp.first);
            return;
        }
    }
}

Component* GameObject::FindComponentWithID(unsigned long long componentID)
{
    for (auto& comp : components)
        if (comp.second->GetComponentID() == componentID)
            return comp.second;
    return nullptr;
}
