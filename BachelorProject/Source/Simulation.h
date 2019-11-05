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
#include <window/WindowTitle.h>

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
	inline static const std::string emiterMatrixUniform = "u_emiterMatrix";
	inline static const std::string emiterParticlesNumberUnifom = "u_emiterParticlesNumber";
	inline static const std::string emiterVelocityUniform = "u_emiterVelocity";
	inline static bool m_emiterChanges = false;
	inline static glm::mat4 m_emiterMatrix;
	inline static int m_emiterNumOfParticles;
	inline static float m_emiterVelocity;
	inline static const int m_printTimesFrequency = 1024;
	inline static const int m_forTitleTimesFrequency = 16;
	inline static const bool MEASURE_TIME = false;
	inline static const bool PRINT_TIME = false;

	inline static const std::string m_SimFpsTitle = "Sim FPS ";
	inline static const std::string m_SimParticleTitle = "Sim Num of particles";
	inline static const std::string m_SimGlassParticleTitle = "Sim Num of Glass particles";

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

	static void setEmiterMatrix(const glm::mat4& matrix);
	static void setEmiterNumOfParticles(const int numOfParticles);
	static void setEmiterVelocity(const float velocity);
};

