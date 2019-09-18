#pragma once
#include <atomic>
#include <Configuration.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Logger.h>
#include <shaders/ComputeShader.h>
#include <dataStructures/ParticleData.h>
#include <particleObjects/ParticleObjectCreator.h>
#include <Utils.h>
#include <condition_variable>
#include <mutex>
#include <thread>
#define _USE_MATH_DEFINES
#include <math.h>

enum Resource_Request { NO_ORDER, OPEN, COMMIT, COMMIT_AND_OPEN };

class Simulation
{
	inline static GLFWwindow* m_simulationWindow;
	inline static std::thread m_simulationThread;

	inline static ComputeShader m_CellCounting;

	inline static ComputeShader m_TESTshader;
public:

	inline static std::atomic<Resource_Request> m_reqFluidArray		= NO_ORDER;
	inline static std::atomic<Resource_Request> m_reqGlassArray		= NO_ORDER;
	inline static std::atomic<Resource_Request> m_reqGlassVectorsArray = NO_ORDER;
	inline static std::atomic<Resource_Request> m_reqDetils		= NO_ORDER;
	inline static std::atomic<Resource_Request> m_reqObjects	= NO_ORDER;


	// calculate 1 frame of simulation 
	static void runSimulationFrame();

	static void startSimulation(GLFWwindow* baseWindow);

	static void main();

	static void init();

	// check m_ResourceRequest and perform action
	static void parseResourceRequest();
};

