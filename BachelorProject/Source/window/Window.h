#ifndef WINDOW_H
#define WINDOW_H


#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "./listener/MousePositionListener.h"
#include "./listener/MouseScrollListener.h"
#include "./listener/WindowSizeListener.h"
#include "./listener/KeyPressListener.h"

class Window {
public:
	Window(int width, int height, std::string name);
	~Window();
	bool init();
	bool refresh();
	void processInput();
	

	void subscribeForMousePositionChanges(MousePositionListener* listener);
	void subscribeForMouseScrollChanges(MouseScrollListener* listener);
	void subscribeForWindowSizeChanges(WindowSizeListener* listener);
	void subscribeForKeyInput(KeyPressListener* listener);
	void unsubscribeMousePositionListener(MousePositionListener* listener);
	void unsubscribeMouseScrollListener(MouseScrollListener* listener);
	void unsubscribeWindowSizeListener(WindowSizeListener* listener);
	void unsubscribeKeyInputListener(KeyPressListener* listener);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	float getDeltaTime() const { return deltaTime; }

	GLFWwindow* glfwWindow;
protected:
private:
	
	int width;
	int height;
	std::string name;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool firstMousePositionMeasurment { true };
	double mousePosX;
	double mousePosY;
	std::vector<MousePositionListener*> mousePositionListeners;
	std::vector<MouseScrollListener*> mouseScrollListeners;
	std::vector<WindowSizeListener*> windowSizeListeners;
	std::vector<KeyPressListener*> keyInputListeners;
	float lastKeyPressTime;

	static void GlfwWindowResizeCallback(GLFWwindow* window, int width, int height);
	static void GlfwWindowMouseMoveCallback(GLFWwindow* window, double x, double y);
	static void GlfwWindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void GLAPIENTRY GlfwWindowMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	);

	void handleResize(int width, int height);
	void handleMouseMove(double x, double y);
	void handleMouseScroll(double xoffset, double yoffset);
};

#endif // !WINDOW_H
