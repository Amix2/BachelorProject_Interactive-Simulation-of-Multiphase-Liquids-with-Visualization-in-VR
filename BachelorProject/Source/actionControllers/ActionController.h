#pragma once
#include <selectableObject/SelectableObject.h>
#include <Logger.h>
#include <string>

class ActionController {
protected:

	void defaultAction(std::string name = "") const { LOG_F(WARNING, "Action %s not supported", name.c_str()); }

};