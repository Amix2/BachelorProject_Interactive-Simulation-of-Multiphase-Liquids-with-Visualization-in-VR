#pragma once
#include <glm/glm.hpp>
#include <actionControllers/VRActionController.h>
#include <memory>


class SelectableObject {
public:
	virtual float getSelectingRadius() const { return m_SelectingRadius; }
	virtual bool isSelected() const = 0;
	virtual bool canBeSelected() const { return !isSelected(); }
	virtual void grab() = 0;
	virtual void release() = 0;
	virtual const glm::mat4* getMatrix() = 0;
	virtual void setMatrix(const glm::mat4 &matrix) = 0;

	VRActionController* getVRActionController() const { return m_vrActionController.get(); }
	//template<class T>
	//SelectableObject(T* object) {
	//	setVRActionController<T>(object);
	//}

protected:
	SelectableObject() {
		setVRActionController();
	}

	void setVRActionController() { m_vrActionController = std::make_shared<VRActionController>(); }

	template<class T, class V> void setVRActionController(V* object) { m_vrActionController = std::make_shared<T>(object); }

	std::shared_ptr<VRActionController> m_vrActionController;

	float m_SelectingRadius = 0;
};