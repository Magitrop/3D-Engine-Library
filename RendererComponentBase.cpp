#include <iostream>
#include <algorithm>
#include "pch.h"

#include "RendererComponentBase.h"
#include "ObjectsManager.h"

void RendererComponentBase::OnDestroy()
{
	auto it = std::find(ObjectsManager->renderQueue.begin(), ObjectsManager->renderQueue.end(), this);
	if (it != ObjectsManager->renderQueue.end())
		ObjectsManager->renderQueue.erase(it);
}

void RendererComponentBase::OnComponentAdded()
{
    auto it = std::find(ObjectsManager->renderQueue.begin(), ObjectsManager->renderQueue.end(), this);
    if (it == ObjectsManager->renderQueue.end())
        ObjectsManager->renderQueue.push_back(this);
}

void RendererComponentBase::Serialize(Json& writeTo)
{
    writeTo[GetComponentName()]["castShadows"] = castShadows;
    writeTo[GetComponentName()]["receiveShadows"] = receiveShadows;
    writeTo[GetComponentName()]["[hide]renderQueueIndex"] = renderQueueIndex;
}

void RendererComponentBase::Deserialize(const Json& readFrom)
{
    castShadows = readFrom[GetComponentName()]["castShadows"];
    receiveShadows = readFrom[GetComponentName()]["receiveShadows"];
    renderQueueIndex = readFrom[GetComponentName()]["[hide]renderQueueIndex"];
}

std::string RendererComponentBase::GetComponentName()
{
    return "Renderer";
}
