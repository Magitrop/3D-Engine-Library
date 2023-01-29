#pragma once
#include <nlohmann/json.hpp>
using Json = nlohmann::json;

struct JsonSerializable
{
	virtual void Serialize(Json& writeTo) = 0;
	virtual void Deserialize(const Json& readFrom) = 0;
};