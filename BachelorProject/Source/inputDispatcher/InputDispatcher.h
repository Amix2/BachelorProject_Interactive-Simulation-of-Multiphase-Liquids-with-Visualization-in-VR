#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <unordered_set>
#include "KeyState.h"
#include <inputDispatcher/listener/KeyInputListener.h>
#include <inputDispatcher/listener/MousePositionListener.h>
#include <inputDispatcher/listener/MouseScrollListener.h>



class InputDispatcher
{
public:
	InputDispatcher();

	void handleButtonAction(int button, int action, float deltaTime);
	void handleMousePositionChange(float x, float y);
	void handleMouseScroll(float scroll);
	void dispatchInput(float deltaTime);

	void subscribeForKeyInput(KeyInputListener* listener, const std::vector<int>& keys);
	void subscribeForKeyInput(KeyInputListener* listener, const int key);
	void subscribeForMousePositionChanges(MousePositionListener* listener) { mousePositionListeners.push_back(listener); }
	void subscribeForMouseScroll(MouseScrollListener* listener) { mouseScrollListeners.push_back(listener); }
private:
	//Key Input
	std::unordered_map<int, std::unordered_set<KeyInputListener*>*> keysToListeners;
	std::unordered_set<int> pressedKeys;

	//Mouse position
	std::vector<MousePositionListener*> mousePositionListeners;

	//Mouse scroll
	std::vector<MouseScrollListener*> mouseScrollListeners;

};

