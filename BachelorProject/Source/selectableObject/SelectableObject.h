#pragma once
#include <glm/glm.hpp>

class SelectableObject {
public:
	virtual bool isSelected() const = 0;
	bool canBeSelected() const { return !isSelected(); }
	virtual void grab() = 0;
	virtual void release() = 0;
	virtual const glm::mat4* getMatrix() = 0;
	virtual void setMatrix(const glm::mat4 &matrix) = 0;
};