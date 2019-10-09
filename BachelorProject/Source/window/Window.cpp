#include "Window.h"
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

	//TODO: Check errors
	glewInit();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(Window::GlfwWindowMessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
		GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Started debugging");


	return true;
}

bool Window::refresh()
{
	glfwSwapBuffers(glfwWindow);

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glfwPollEvents();
	processInput();

	return glfwWindowShouldClose(glfwWindow);
}

#include <scene/camera/SimpleCameraController.h>
#include <particleObjects/ParticleObjectCreator.h>
#include <particleObjects/ParticleObjectManager.h>

void Window::processInput() 
{
	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwWindow, true);

	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_W);
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_S);
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_A);
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_D);
	if (glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS)
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_Q);
	if (glfwGetKey(glfwWindow, GLFW_KEY_E) == GLFW_PRESS)
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_E);
	if (glfwGetKey(glfwWindow, GLFW_KEY_1) == GLFW_PRESS && glfwGetTime() - lastKeyPressTime > 0.5f) {
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_1);
		lastKeyPressTime = glfwGetTime();
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS && glfwGetTime() - lastKeyPressTime > 0.5f) {
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_2);
		lastKeyPressTime = glfwGetTime();
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_3) == GLFW_PRESS && glfwGetTime() - lastKeyPressTime > 0.5f) {
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_3);
		lastKeyPressTime = glfwGetTime();
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_4) == GLFW_PRESS && glfwGetTime() - lastKeyPressTime > 0.5f) {
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_4);
		lastKeyPressTime = glfwGetTime();

		Scene::Camera* camera = SimpleCameraController::cam;
		glm::vec3 low = camera->getPosition() + (camera->getFront() * 25.0f) + glm::vec3(-3, -3, -3);
		glm::vec3 high = camera->getPosition() + (camera->getFront() * 25.0f) + glm::vec3(3, 3, 3);
		ParticleObjectDetais newObj{ 1, low.x, low.y, low.z, high.x, high.y, high.z };
		ParticleObjectCreator::addObject(newObj);
		
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_5) == GLFW_PRESS) {
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_5);

		ParticleObjectManager::moveObject(0, true);
	}

	if (glfwGetKey(glfwWindow, GLFW_KEY_6) == GLFW_PRESS) {
		for (KeyPressListener* listener : keyInputListeners)
			listener->handleKeyPress(KEY_6);

		ParticleObjectManager::moveObject(0, false);
	}

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

void Window::subscribeForKeyInput(KeyPressListener* listener)
{
	this->keyInputListeners.push_back(listener);
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

void Window::unsubscribeKeyInputListener(KeyPressListener* listener)
{
	std::remove(keyInputListeners.begin(), keyInputListeners.end(), listener);

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




