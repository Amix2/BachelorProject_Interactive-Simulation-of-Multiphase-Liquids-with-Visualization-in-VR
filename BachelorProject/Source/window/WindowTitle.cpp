#include "WindowTitle.h"

void WindowTitle::workerMain()
{
	while (!glfwWindowShouldClose(m_glfwWindow)) {
		if (m_glfwWindow != nullptr) {
			std::stringstream ss;
			bool afterFirst = false;
			for (int i = 0; i < Configuration.MAX_AMOUNT_OF_TITLES; i++) {
				if (m_titleStringList[i] != DEFEULT_NULL_TITLE) {
					if (afterFirst) ss << ", ";
					ss << m_titleNamesList[i] << " " << m_titleStringList[i];
				}
			}
			glfwSetWindowTitle(m_glfwWindow, ss.str().c_str());
		}
		Sleep(Configuration.TITLE_REFRESH_DELAY);
	}
}

void WindowTitle::init()
{
	WindowTitle::m_simulationThread = std::thread(WindowTitle::workerMain);
	Threads::addThreadToList(&WindowTitle::m_simulationThread);
}

void WindowTitle::setWindow(GLFWwindow* glfwWindow)
{
	m_glfwWindow = glfwWindow;
}

void WindowTitle::addTitle(std::string titleName, int position)
{
	if (m_titleNamesList[position] == WindowTitle::DEFEULT_NULL_TITLE) {	// simple add
		m_titleNamesList[position] = titleName;
		m_indexMap.insert({ titleName, position });
		LOG_F(INFO, "New title added, name: %s, position: %d", titleName.c_str(), position);
	}
	else {	// position is already taken
		while (position < Configuration.MAX_AMOUNT_OF_TITLES && m_titleNamesList[position] == WindowTitle::DEFEULT_NULL_TITLE) {
			position++;
		}
		if (position == Configuration.MAX_AMOUNT_OF_TITLES) {
			LOG_F(ERROR, "ERROR while addking new title, operation failed. name: %s, position: %d", titleName.c_str(), position);
			return;
		}
		m_titleNamesList[position] = titleName;
		m_indexMap.insert({ titleName, position });
		LOG_F(WARNING, "New title added IN DIFFERENT POSITION, name: %s, position: %d", titleName.c_str(), position);
	}
}

void WindowTitle::setTitle(std::string titleName, std::string titleString)
{
	if (WindowTitle::m_indexMap.find(titleName) != WindowTitle::m_indexMap.end()) {	// if contains

		int index = WindowTitle::m_indexMap[titleName];
		m_titleStringList[index] = titleString;
	}
	else {
		LOG_F(ERROR, "no title for given name");
		throw "no title for given name";
	}
}

