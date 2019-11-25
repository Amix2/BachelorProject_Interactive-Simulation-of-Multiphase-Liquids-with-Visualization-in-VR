#include "Window.h"

Window::Window(int width, int height, std::string name, InputDispatcher& inputDispatcher)
	: width { width }
	, height { height } 
	, name { name } 
	, inputDispatcher { &inputDispatcher }
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
		LOG_F(ERROR, "Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwSetWindowUserPointer(glfwWindow, this);

	glfwMakeContextCurrent(glfwWindow);

	glfwSetFramebufferSizeCallback(glfwWindow, GlfwWindowResizeCallback);
	glfwSetCursorPosCallback(glfwWindow, GlfwWindowMouseMoveCallback);
	glfwSetScrollCallback(glfwWindow, GlfwWindowMouseScrollCallback);
	glfwSetMouseButtonCallback(glfwWindow, GlfwWindowMouseButtonCallback);
	glfwSetKeyCallback(glfwWindow, GlfwWindowKeyCallback);


	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewInit();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(Window::GlfwWindowMessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
		GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Started debugging");
}

void Window::refresh()
{
	glfwSwapBuffers(glfwWindow);

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glfwPollEvents();
	inputDispatcher->dispatchInput(deltaTime);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(glfwWindow);
}

void Window::getFrameFrom(const FrameBuffer& frameBuffer)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	frameBuffer.readFrom();

	glBlitFramebuffer(0, 0, frameBuffer.getWidth(), frameBuffer.getHeight(), 0, 0, width, height,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Window::subscribeForFrameBufferSizeChanges(FrameBufferSizeListener* listener)
{
	this->framebufferSizeListeners.push_back(listener);
}

void Window::GlfwWindowMouseMoveCallback(GLFWwindow* window, double x, double y)
{
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->handleMouseMove(x, y);
}

void Window::GlfwWindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->handleMouseScroll(xoffset, yoffset);
}

void Window::GlfwWindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->handleMouseButtonPressed(button, action);
}

void Window::GlfwWindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->handleKeyPressed(key, action);
}

void GLAPIENTRY Window::GlfwWindowMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
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
	for(FrameBufferSizeListener* listener: framebufferSizeListeners)
	{
		listener->handleFrameBufferResize(width, height);
	}
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

	inputDispatcher->handleMousePositionChange(xoffset, yoffset);
}

void Window::handleMouseScroll(double xoffset, double yoffset)
{
	inputDispatcher->handleMouseScroll(yoffset);
}

void Window::handleMouseButtonPressed(int button, int action)
{
	inputDispatcher->handleButtonAction(button, action, deltaTime);
}

void Window::handleKeyPressed(int key, int action)
{
	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwWindow, true);
	else if(action != GLFW_REPEAT)
		inputDispatcher->handleButtonAction(key, action, deltaTime);
}
