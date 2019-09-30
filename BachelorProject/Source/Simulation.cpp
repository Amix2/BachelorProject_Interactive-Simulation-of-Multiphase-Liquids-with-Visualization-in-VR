#include "Simulation.h"

void setupSimObjects();

const std::string stageUniform = "u_stage";
const std::string turnUniform = "u_turnInStage";

void Simulation::runSimulationFrame()
{
	std::chrono::time_point<std::chrono::steady_clock> 
		_ntStart, _ntParseRequests, _ntSynchronizeWithGpu, _ntCopyForSort, _ntCellCounting, _ntBitonicSort, _ntArrangeVars;
	float _ntStartTime, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime;
	_ntStartTime = _ntParseRequestsTime = _ntSynchronizeWithGpuTime = _ntCopyForSortTime = _ntCellCountingTime = _ntBitonicSortTime = _ntArrangeVarsTime = 0;
	for (int i = 0; i < 1000; i++) {
		glFinish();
		_ntStart = getNanoTime();


		// open or close resources required by other threads
		parseResourceRequest();
		if (ParticleData::m_OpenedResources > 0) return;

		glFinish();		_ntParseRequests = getNanoTime();		_ntParseRequestsTime += getNanoTimeDif(_ntStart, _ntParseRequests);

		// exchange information about glass objects with gpu
		ParticleObjectManager::synchronizeWithGpu();

		glFinish();		_ntSynchronizeWithGpu = getNanoTime();		_ntSynchronizeWithGpuTime += getNanoTimeDif(_ntParseRequests, _ntSynchronizeWithGpu);

		ParticleData::copyDataForSorting();

		glFinish();		_ntCopyForSort = getNanoTime();		_ntCopyForSortTime += getNanoTimeDif(_ntSynchronizeWithGpu, _ntCopyForSort);

		////////////////////////////////////////////////
		//	SHADERS 
		const int dispathSize = ceil(ParticleData::m_FluidParticlesNum / 256.0);

		// Move glass particles


		// ASSIGN cells to particles
		m_CellCounting.runShader(dispathSize, 1, 1, false);

		glFinish(); _ntCellCounting = getNanoTime();		_ntCellCountingTime += getNanoTimeDif(_ntCopyForSort, _ntCellCounting);

		// SORT
		const int bitonicSortWorkGroups = ceil(pow(2, ceil(log2(ParticleData::m_FluidParticlesNum))) / 256.0);	// min power of 2 more than num of particles threads / 256 threads per WorkGroup
		const int numOfStages = ceil(log2(ParticleData::m_FluidParticlesNum));
		for (int currentStage = 1; currentStage <= numOfStages; currentStage++) {
			for (int currentTurn = 1; currentTurn <= currentStage; currentTurn++) {
				m_BitonicSort.setUniformVariable(stageUniform, currentStage);
				m_BitonicSort.setUniformVariable(turnUniform, currentTurn);
				m_BitonicSort.runShader(dispathSize, 1, 1, false);
			}
		}
		glFinish();		_ntBitonicSort = getNanoTime();		_ntBitonicSortTime += getNanoTimeDif(_ntCellCounting, _ntBitonicSort);
		// ARRANGE variables after sorting
		m_VariablesArrangement.runShader(dispathSize, 1, 1, false);
		glFinish();		_ntArrangeVars = getNanoTime();		_ntArrangeVarsTime += getNanoTimeDif(_ntBitonicSort, _ntArrangeVars);

		m_TESTshader.runShader(1, 1, 1, true);

	}

	checkOpenGLErrors();
	//ParticleData::printParticleData(20);

	////////////////////////////////////////////////

	
	// if enabled - log all particles positions into file
	if (LOG_TO_FILE) {
		ParticleData::logParticlePositions();
	}

	LOG_F(INFO, "Simulation time: Requests %f, SyncGPU %f,  CopyBO %f, CellCounting %f, Sort %f, Arrange %f"
		, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime);
}

void Simulation::startSimulation(GLFWwindow* baseWindow)
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_simulationWindow = glfwCreateWindow(10, 10, "OTHER", NULL, baseWindow);
	m_mainWindow = baseWindow;
	Simulation::m_simulationThread =  std::thread(Simulation::main);
	Threads::addThreadToList(&Simulation::m_simulationThread);
}

void setupSimObjects()
{
	ParticleObjectDetais details{ 1, 1,1,1, 1.1, 1.1, 10.1 };
	ParticleObjectDetais detailsQ{ 2, 1,1,1, 1.1, 1.1, 10.1 };
	ParticleObjectDetais details2{ 2, 10,10,10, 10.1,60.1, 20 };
	ParticleObjectDetais details3{ -1, 5,4,5, 2.5,0,2.5 };
	ParticleObjectCreator::addObject(details);
	//ParticleObjectCreator::addObject(detailsQ);
	//ParticleObjectCreator::addObject(details2);
	//ParticleObjectCreator::addObject(details3);
}

void Simulation::main()
{
	loguru::set_thread_name("Sim");
	LOG_F(INFO, "Simulation Thread START");
	glfwMakeContextCurrent(Simulation::m_simulationWindow);
	Simulation::init();
	ParticleData::initArraysOnGPU();

	setupSimObjects();
	checkOpenGLErrors();

	while (!glfwWindowShouldClose(m_mainWindow))
	{
		// run simulation 1 turn
		Simulation::runSimulationFrame();
		//ParticleData::printGlassData(20);
		//ParticleData::printSortingData();
	}
		ParticleData::printParticleData(20);
		
}

void Simulation::init()
{
	m_TESTshader = ComputeShader(ShaderFiles.TEST_ComputeShader);
	m_CellCounting = ComputeShader(ShaderFiles.CellCountingForSort);
	m_BitonicSort = ComputeShader(ShaderFiles.BitonicSort);
	m_VariablesArrangement = ComputeShader(ShaderFiles.VariablesArrangementAfterSort);
}

void Simulation::parseResourceRequest()
{

	switch (m_reqFluidArray) {
	case OPEN:
		ParticleData::openFluidArray();
		m_reqFluidArray = NO_ORDER;
		break;
	case COMMIT:
		ParticleData::commitFluidArray();
		m_reqFluidArray = NO_ORDER;
		break;
	case NO_ORDER:
		break;
	}

	switch (m_reqGlassArray) {
	case OPEN:
		ParticleData::openGlassArray();
		m_reqGlassArray = NO_ORDER;
		break;
	case COMMIT:
		ParticleData::commitGlassArray();
		m_reqGlassArray = NO_ORDER;
		break;
	case NO_ORDER:
		break;
	}

	switch (m_reqGlassVectorsArray) {
	case OPEN:
		ParticleData::openGlassVectors();
		m_reqGlassVectorsArray = NO_ORDER;
		break;
	case COMMIT:
		ParticleData::commitGlassVectors();
		m_reqGlassVectorsArray = NO_ORDER;
		break;
	case NO_ORDER:
		break;
	}

	switch (m_reqDetils) {
	case OPEN:
		ParticleData::openDetails();
		m_reqDetils = NO_ORDER;
		break;
	case COMMIT:
		ParticleData::commitDetails();
		m_reqDetils = NO_ORDER;
		break;
	case NO_ORDER:
		break;
	}

	switch (m_reqObjects) {
	case OPEN:
		ParticleData::openObjects();
		m_reqObjects = NO_ORDER;
		break;
	case COMMIT:
		ParticleData::commitObjects();
		m_reqObjects = NO_ORDER;
		break;
	case COMMIT_AND_OPEN:
		ParticleData::commitObjects();
		ParticleData::openObjects();
		m_reqObjects = NO_ORDER;
	case NO_ORDER:
		break;
	}

	checkOpenGLErrors();
}

