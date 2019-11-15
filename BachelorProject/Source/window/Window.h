#ifndef WINDOW_H
#define WINDOW_H


#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <window/listener/WindowSizeListener.h>
#include <inputDispatcher/InputDispatcher.h>

class Window {
public:
	Window(int width, int height, std::string name, InputDispatcher& inputDispatcher);
	~Window();
	bool init();
	bool refresh();	

	void subscribeForWindowSizeChanges(WindowSizeListener* listener);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	float getDeltaTime() const { return deltaTime; }

	GLFWwindow* glfwWindow = nullptr;
protected:
private:
	InputDispatcher* inputDispatcher;
	
	int width;
	int height;
	std::string name;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool firstMousePositionMeasurment { true };
	double mousePosX;
	double mousePosY;

	std::vector<WindowSizeListener*> windowSizeListeners;

	static void GlfwWindowResizeCallback(GLFWwindow* window, int width, int height);
	static void GlfwWindowMouseMoveCallback(GLFWwindow* window, double x, double y);
	static void GlfwWindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void GlfwWindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void GlfwWindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
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
	void handleMouseButtonPressed(int button, int action);
	void handleKeyPressed(int key, int action);
	//void handleControllerButtonPressed(int button,)
};

#endif // !WINDOW_H
