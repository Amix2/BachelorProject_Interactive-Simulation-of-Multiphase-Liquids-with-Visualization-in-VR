#pragma once
#include <vector>
#include <selectableObject/SelectableObject.h>
#include <Logger.h>
#include <glm/gtx/string_cast.hpp>
#include <Configuration.h>

class SelectableObjectManager
{
public:
	inline static std::vector<SelectableObject*> m_selectableObjects;

	inline static void init();
	static void addSelectableObject(SelectableObject* object);
	static void print();
};

