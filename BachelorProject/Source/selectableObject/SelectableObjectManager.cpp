#include "SelectableObjectManager.h"

inline void SelectableObjectManager::init()
{
	m_selectableObjects.reserve(Configuration.MAX_SELECTABLE_OBJECTS);
}

void SelectableObjectManager::addSelectableObject(SelectableObject* object)
{
	m_selectableObjects.push_back(object);
}

void SelectableObjectManager::print()
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Selectable Objects print");
	int i = 0;
	for (SelectableObject* object : m_selectableObjects) {
		bool test = object->isSelected();
		const glm::mat4* mat = object->getMatrix();
		LOG_F(INFO, "%d:\tselected: %s, \n\tmatrix: %s", i, object->isSelected() ? "true" : "false",  glm::to_string(*(mat)).c_str());
	}

	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
