#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <assert.h>
#include <map>

union MultiTypeValue {
	bool boolValue;
	int intValue;
	float floatValue;
	glm::vec4 vec4Value;
};

enum class Params {
	COLOR
};


class MaterialObjectOwner
{
public:
	virtual glm::mat4 getModel() const = 0;

	virtual std::map<Params, MultiTypeValue> getAdditionalParameters() const { return std::map<Params, MultiTypeValue>(); }

};