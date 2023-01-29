#pragma once
#include <string>
#include <map>

#define SINGLETON(name) \
protected: \
	friend class SingletonManager; \
	name() = default; \
	name(name&) = delete; \
	name& operator = (name&) = delete; \

class SingletonBase
{
protected:
	SingletonBase() = default;
};

class SingletonManager final
{
	friend class EngineEditor;

	SingletonManager() = default;

	std::map<std::string, SingletonBase*> singletons;
public:
	template<class T, class = std::enable_if<std::is_base_of<SingletonBase, T>::value>>
	T*& Get()
	{
		std::string name = typeid(T).name();
		if (singletons.find(name) == singletons.end())
		{
			T* t = new T();
			singletons[name] = t;
		}
		return (T*&)singletons[name];
	}

	static SingletonManager*& Instance()
	{
		static SingletonManager* _instance;
		if (!_instance) _instance = new SingletonManager();
		return _instance;
	}
};