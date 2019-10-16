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
#include <math.h>

//enum Resource_Request { NO_ORDER, OPEN, COMMIT, COMMIT_AND_OPEN };

class Simulation
{
	inline static GLFWwindow* m_simulationWindow;
	inline static GLFWwindow* m_mainWindow;
	inline static std::thread m_simulationThread;

	inline static ComputeShader m_CellCounting;
	inline static ComputeShader m_BitonicSort;
	inline static ComputeShader m_VariablesArrangement;
	inline static ComputeShader m_SphNeighbourSearch;
	inline static ComputeShader m_SphDensityPressureFluid;
	inline static ComputeShader m_SphAccelerationFluid;
	inline static ComputeShader m_SphVelocity;

	inline static unsigned int m_turnNumber = 0;

	inline static float _ntSum, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime, _ntNeighbourSearchTime, _ntSyncDetailsTime, _ntDensityPressureFluidTime, _ntAccelerationFluidTime, _ntVelocityTime, _ntRangeCalc;

	inline static const std::string stageUniform = "u_stage";
	inline static const std::string turnUniform = "u_turnInStage";
	inline static const int printTimesFrequency = 64;
	inline static const bool MEASURE_TIME = false;

public:

	//inline static std::atomic<Resource_Request> m_reqFluidArray		= NO_ORDER;
	//inline static std::atomic<Resource_Request> m_reqGlassArray		= NO_ORDER;
	//inline static std::atomic<Resource_Request> m_reqGlassVectorsArray = NO_ORDER;
	//inline static std::atomic<Resource_Request> m_reqDetils		= NO_ORDER;
	//inline static std::atomic<Resource_Request> m_reqObjects	= NO_ORDER;


	// calculate 1 frame of simulation 
	static void runSimulationFrame();

	static void startSimulation(GLFWwindow* baseWindow);

	static void main();

	static void init();

	static void handlePrintingTimes();

	static void setupSimObjects();

};

