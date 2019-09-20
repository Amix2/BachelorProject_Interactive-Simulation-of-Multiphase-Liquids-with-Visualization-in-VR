#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "MousePositionListener.h"
#include "MouseScrollListener.h"
#include "WindowSizeListener.h"

class Window {
public:
	Window(int width, int height, std::string name);
	~Window();
	bool init();
	bool refresh();
	void processInput() const;

	void subscribeForMousePositionChanges(MousePositionListener* listener);
	void subscribeForMouseScrollChanges(MouseScrollListener* listener);
	void subscribeForWindowSizeChanges(WindowSizeListener* listener);
	void unsubscribeMousePositionListener(MousePositionListener* listener);
	void unsubscribeMouseScrollListener(MouseScrollListener* listener);
	void unsubscribeWindowSizeListener(WindowSizeListener* listener);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
protected:
private:
	GLFWwindow* glfwWindow;

	int width;
	int height;
	std::string name;

	bool firstMousePositionMeasurment { true };
	double mousePosX;
	double mousePosY;
	std::vector<MousePositionListener*> mousePositionListeners;
	std::vector<MouseScrollListener*> mouseScrollListeners;
	std::vector<WindowSizeListener*> windowSizeListeners;

	static void GlfwWindowResizeCallback(GLFWwindow* window, int width, int height);
	static void GlfwWindowMouseMoveCallback(GLFWwindow* window, double x, double y);
	static void GlfwWindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	void handleResize(int width, int height);
	void handleMouseMove(double x, double y);
	void handleMouseScroll(double xoffset, double yoffset);
};

#endif // !WINDOW_H
