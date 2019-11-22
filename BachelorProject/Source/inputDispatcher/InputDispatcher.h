#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <OpenVR/openvr.h>

#include <unordered_map>
#include <unordered_set>
#include "KeyState.h"
#include <vrInputProviders/Provider.h>
#include <inputDispatcher/listener/KeyInputListener.h>
#include <inputDispatcher/listener/MousePositionListener.h>
#include <inputDispatcher/listener/MouseScrollListener.h>
#include <inputDispatcher\listener\ControllerInputListener.h>
#include <memory>


class InputDispatcher
{
public:
	InputDispatcher();

	void handleButtonAction(int button, int action, float deltaTime);
	void handleMousePositionChange(float x, float y);
	void handleMouseScroll(float scroll);
	void handleControllerInput(const vr::EVREventType eventType, const vr::EVRButtonId buttonId);
	void dispatchInput(float deltaTime);

	void subscribeForKeyInput(KeyInputListener* listener, const std::vector<int>& keys);
	void subscribeForKeyInput(KeyInputListener* listener, const int key);
	void subscribeForMousePositionChanges(MousePositionListener* listener) { mousePositionListeners.push_back(listener); }
	void subscribeForMouseScroll(MouseScrollListener* listener) { mouseScrollListeners.push_back(listener); }
private:
	//Key Input
	std::unordered_map<int, std::unordered_set<KeyInputListener*>*> keysToListeners;
	std::unordered_set<int> pressedKeys;
	std::unordered_map<std::string, std::shared_ptr<Provider>> providers;

	//Mouse position
	std::vector<MousePositionListener*> mousePositionListeners;

	//Mouse scroll
	std::vector<MouseScrollListener*> mouseScrollListeners;

	//Controller input
	std::vector<std::shared_ptr<ControllerInputListener>> controllerInputListeners;

};

