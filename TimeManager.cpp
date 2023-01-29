#include "pch.h"
#include "TimeManager.h"

double TimeSingleton::GetTime() const
{
	return time;
}

double TimeSingleton::GetDeltaTime() const
{
	return deltaTime;
}
