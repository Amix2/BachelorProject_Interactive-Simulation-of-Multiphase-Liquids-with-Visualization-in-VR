#include "InputDispatcher.h"

InputDispatcher::InputDispatcher(){}

void InputDispatcher::handleButtonAction(int button, int action, float deltaTime)
{
	switch (action) 
	{
	case GLFW_PRESS: {
		pressedKeys.insert(button);
		std::unordered_set<KeyInputListener*>* listeners = keysToListeners[button];
		if (listeners != nullptr)
			for (KeyInputListener* listener : *listeners)
				listener->handleKeyPress(button, KeyState::FALLING_EDGE, deltaTime);
		break;
	}
	case GLFW_RELEASE: 
	{
		pressedKeys.erase(button);
		std::unordered_set<KeyInputListener*>* listeners = keysToListeners[button];
		if (listeners != nullptr)
			for (KeyInputListener* listener : *listeners)
				listener->handleKeyPress(button, KeyState::RISING_EDGE, deltaTime);
		break;
	}
	default:
		break;
	}
}

void InputDispatcher::handleMousePositionChange(float x, float y)
{
	for (auto listener : mousePositionListeners)
		listener->handleMouseMove(x, y);
}

void InputDispatcher::handleMouseScroll(float scroll)
{
	for (auto listener : mouseScrollListeners)
		listener->handleMouseScroll(scroll);
}

void InputDispatcher::dispatchInput(float deltaTime)
{
	for (int button : pressedKeys) {
		std::unordered_set<KeyInputListener*>* listeners = keysToListeners[button];
		if (listeners != nullptr)
			for (KeyInputListener* listener : *listeners)
				listener->handleKeyPress(button, KeyState::PRESSED, deltaTime);
	}
}

void InputDispatcher::subscribeForKeyInput(KeyInputListener* listener, const std::vector<int>& keys)
{
	for (int key : keys) 
		subscribeForKeyInput(listener, key);
}

void InputDispatcher::subscribeForKeyInput(KeyInputListener* listener, const int key)
{
	if (keysToListeners[key] == nullptr)
		keysToListeners[key] = new std::unordered_set<KeyInputListener*>{ listener };
	else
		keysToListeners[key]->insert(listener);
}
