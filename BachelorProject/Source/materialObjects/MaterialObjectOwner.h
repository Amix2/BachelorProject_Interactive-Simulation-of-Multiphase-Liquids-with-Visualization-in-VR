#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <assert.h>
#include <map>

union MultiTypeValue {
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
	//virtual void fillParameters(std::vector<MultiTypeValue>& values) const { if (values.size() != 0) throw "MaterialObjectOwner does not support additional parameters"; }
	//bool supportsAdditionalParameters() const {
	//	try {
	//		std::vector< MultiTypeValue> vec;
	//		vec.resize(1);
	//		fillParameters(vec);
	//	}
	//	catch (...) {
	//		return false;
	//	}
	//	return true;
	//}

	virtual std::map<Params, MultiTypeValue> getAdditionalParameters() const { return std::map<Params, MultiTypeValue>(); }

};