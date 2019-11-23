#pragma once
#include <materialObjects/MaterialObject.h>

class MaterialObjectOwner : public MaterialObject {
public:
	virtual glm::mat4 getModel() const = 0;
};