#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <thread>
#include <Logger.h>
#include <Configuration.h>
#include <ThreadManager.h>
#include <sstream>
#include <Windows.h>
#include <algorithm>

class WindowTitle
{
private:
	inline static const std::string DEFEULT_NULL_TITLE = std::string("NULL_TITLE_DO_NOT_USE_AS_TITLE");

	inline static std::unordered_map<std::string, int> m_indexMap;
	inline static std::string m_titleStringList[Configuration.MAX_AMOUNT_OF_TITLES] = { "" };
	inline static std::string m_titleNamesList[Configuration.MAX_AMOUNT_OF_TITLES];


	inline static GLFWwindow* m_glfwWindow;

	inline static std::thread m_simulationThread;

	static void workerMain();

public:

	static void init();

	static void setWindow(GLFWwindow* glfwWindow);

	static void addTitle(std::string titleName, int position);

	static void setTitle(std::string titleName, std::string titleString);
};

