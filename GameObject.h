#pragma once
#include "pch.h"

#include "Component.h"
#include "TransformComponent.h"
#include "EventSystem.h"

#include <map>
#include <string>
#include <list>
#include <typeinfo>
#include <iostream>

class GameObject final 
{
    friend class EngineEditor;
    friend class ObjectsManagerSingleton;
    friend class EventSystemSingleton;
    friend class EventsControllerSingleton;
    friend void Component::OnComponentAdded();

    std::map<std::string, Component*> components;
    explicit GameObject();

    void operator delete(void* ptr)
    {
        ::operator delete(ptr);
    }

    void operator delete[](void* ptr)
    {
        ::operator delete[](ptr);
    }

    unsigned long long gameObjectID = 0;

    bool shouldBeSerialized = true;
    Json Serialize() const;
    void RemoveComponent(std::string componentName);
    Component* FindComponentWithID(unsigned long long componentID);
public:
    std::string name;
    TransformComponent* const transform;
    bool isActive = true;

    ~GameObject();

    unsigned long long GetGameObjectID() const;

    template<class T, class =
        typename std::enable_if<
        std::is_base_of<Component, T>::value>::type>
        T* AddComponent()
    {
        if (//std::is_base_of<MustBeUniqueComponentAttribute, T>::value &&
            HasComponent<T>())
            return GetComponent<T>();

        components[typeid(T).name()] = new T(this);
        if (EventSystem->InPlayMode())
            components[typeid(T).name()]->OnCreate();
        return static_cast<T*>(components[typeid(T).name()]);
    }

    template<class T, class =
        typename std::enable_if<
        std::is_base_of<Component, T>::value>::type>
        T* GetComponent()
    {
        if (!HasComponent<T>())
            return nullptr;

        return static_cast<T*>(components[typeid(T).name()]);
    }

    template<class T, class =
        typename std::enable_if<
        std::is_base_of<Component, T>::value>::type>
        bool HasComponent()
    {
        return components.find(typeid(T).name()) != components.end();
    }

    template<class T, class =
        typename std::enable_if<
        std::is_base_of<Component, T>::value>::type>
        void RemoveComponent()
    {
        static_assert(!std::is_base_of<CannotBeRemovedComponentAttribute, T>::value, "This component cannot be removed.");

        auto componentName = typeid(T).name();
        if (components.find(componentName) == components.end()) 
            return;

        delete components[componentName];
        components.erase(componentName);
    }
};