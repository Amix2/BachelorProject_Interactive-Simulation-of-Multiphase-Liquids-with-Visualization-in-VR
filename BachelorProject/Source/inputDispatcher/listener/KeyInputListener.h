#pragma once
#include "../KeyState.h"

class KeyInputListener {
public:
	void virtual handleKeyPress(int key, KeyState state, float deltaTime) = 0;
};