#include "Window.h"

Window::Window(int width, int height, std::string name)
	: width { width }
	, height { height } 
	, name { name } 
	, glfwWindow { nullptr }
	, mousePosX { -1 }
	, mousePosY { -1 } {}

Window::~Window()
{
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

bool Window::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindow = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (glfwWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwSetWindowUserPointer(glfwWindow, this);

	glfwMakeContextCurrent(glfwWindow);

	glfwSetFramebufferSizeCallback(glfwWindow, GlfwWindowResizeCallback);
	glfwSetCursorPosCallback(glfwWindow, GlfwWindowMouseMoveCallback);
	glfwSetScrollCallback(glfwWindow, GlfwWindowMouseScrollCallback);

	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD";
		exit(1);
	}

	return true;
}

bool Window::refresh()
{
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
	return glfwWindowShouldClose(glfwWindow);
}

void Window::processInput() const
{

}

void Window::subscribeForMousePositionChanges(MousePositionListener* listener)
{
	this->mousePositionListeners.push_back(listener);
}

void Window::subscribeForMouseScrollChanges(MouseScrollListener* listener)
{
	this->mouseScrollListeners.push_back(listener);
}

void Window::subscribeForWindowSizeChanges(WindowSizeListener* listener)
{
	this->windowSizeListeners.push_back(listener);
}

void Window::unsubscribeMousePositionListener(MousePositionListener* listener)
{
	std::remove(mousePositionListeners.begin(), mousePositionListeners.end(), listener);
}

void Window::unsubscribeMouseScrollListener(MouseScrollListener* listener)
{
	std::remove(mouseScrollListeners.begin(), mouseScrollListeners.end(), listener);
}

void Window::unsubscribeWindowSizeListener(WindowSizeListener* listener)
{
	std::remove(windowSizeListeners.begin(), windowSizeListeners.end(), listener);
}

void Window::GlfwWindowMouseMoveCallback(GLFWwindow* window, double x, double y)
{
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->handleMouseMove(x, y);
}

void Window::GlfwWindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->handleMouseMove(xoffset, yoffset);
}

void Window::GlfwWindowResizeCallback(GLFWwindow* window, int width, int height)
{
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->handleResize(width, height);
}

void Window::handleResize(int width, int height)
{
	this->width = width;
	this->height = height;
	if (!windowSizeListeners.empty())
		for_each(windowSizeListeners.begin(), windowSizeListeners.end(), [width, height](WindowSizeListener* listener)
		{
			listener->handleWindowResize(width, height);
		});
}

void Window::handleMouseMove(double x, double y)
{
	if (firstMousePositionMeasurment)
	{
		firstMousePositionMeasurment = false;
		mousePosX = x;
		mousePosY = y;
	}

	double xoffset = x - mousePosX;
	double yoffset = mousePosY - y;

	mousePosX = x;
	mousePosY = y;

	if (!mousePositionListeners.empty())
		for_each(mousePositionListeners.begin(), mousePositionListeners.end(), [xoffset, yoffset](MousePositionListener* listener)
		{
			listener->handleMouseMove(xoffset, yoffset);
		});
}

void Window::handleMouseScroll(double xoffset, double yoffset)
{
	if (!mouseScrollListeners.empty())
		for_each(mouseScrollListeners.begin(), mouseScrollListeners.end(), [yoffset](MouseScrollListener* listener)
		{
			listener->handleMouseScroll(yoffset);
		});
}


