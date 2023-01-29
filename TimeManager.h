#pragma once
#include "Singleton.h"

#define TimeManager SingletonManager::Instance()->Get<TimeSingleton>()

class TimeSingleton final : SingletonBase
{
	SINGLETON(TimeSingleton);
private:
	friend class EngineEditor;
	friend int main();

	double time;
	double deltaTime;
public:
	// time since launch
	double GetTime() const;
	// time since last frame
	double GetDeltaTime() const;
};
