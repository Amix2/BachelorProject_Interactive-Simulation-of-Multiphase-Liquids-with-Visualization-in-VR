#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "MousePositionListener.h"
#include "MouseScrollListener.h"

class Window {
public:
	Window(int width, int height, std::string name);
	~Window();
	bool init();
	bool refresh();

	void subscribeForMousePositionChanges(MousePositionListener& listener);
	void subscribeForMouseScrollChanges(MouseScrollListener& listener);
protected:
private:
	GLFWwindow* glfwWindow;

	int width;
	int height;
	std::string name;

	bool firstMousePositionMeasurment { true };
	int mousePosX;
	int mousePosY;
	MousePositionListener* mousePositionListener;
	MouseScrollListener* mouseScrollListener;

	static void GlfwWindowResizeCallback(GLFWwindow* window, int width, int height);
	static void GlfwWindowMouseMoveCallback(GLFWwindow* window, double x, double y);
	static void GlfwWindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	void handleResize(int width, int height);
	void handleMouseMove(double x, double y);
	void handleMouseScroll(double xoffset, double yoffset);
};

#endif // !WINDOW_H
