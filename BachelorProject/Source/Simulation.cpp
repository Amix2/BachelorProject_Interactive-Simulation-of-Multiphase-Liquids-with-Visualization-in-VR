#include "Simulation.h"

void setupSimObjects();

const std::string stageUniform = "u_stage";
const std::string turnUniform = "u_turnInStage";

void Simulation::runSimulationFrame()
{
	long tStart = getTime();
	// open or close resources required by other threads
	parseResourceRequest();

	// exchange information about glass objects with gpu
	ParticleObjectManager::synchronizeWithGpu();

	int dispathSize = ceil(ParticleData::m_FluidParticlesNum / 256.0);

	ParticleData::copyDataForSorting();

	////////////////////////////////////////////////
	//	SHADERS 

	// Move glass particles


	// Sort fluid particle array
	auto ntStart = getNanoTime();
	for (int i = 0; i < 10; i++) {
	const int cellCountingWorkGroups = ceil(ParticleData::m_FluidParticlesNum / 256.0);
	//LOG_F(INFO, "CELLS compute shader witn (%d, 1, 1)", cellCountingWorkGroups);
	m_CellCounting.runShader(cellCountingWorkGroups, 1, 1, true);

	const int bitonicSortWorkGroups = ceil(pow(2, ceil(log2(ParticleData::m_FluidParticlesNum))) / 256.0);	// min power of 2 more than num of particles threads / 256 threads per WorkGroup
	//LOG_F(INFO, "SORTING compute shader witn (%d, 1, 1)", bitonicSortWorkGroups);
	const int numOfStages = ceil(log2(ParticleData::m_FluidParticlesNum));
	for (int currentStage = 1; currentStage <= numOfStages; currentStage++) {
		for (int currentTurn = 1; currentTurn <= currentStage; currentTurn++) {
			m_BitonicSort.setUniformVariable(stageUniform, currentStage);
			m_BitonicSort.setUniformVariable(turnUniform, currentTurn);
			m_BitonicSort.runShader(dispathSize, 1, 1, false);
		}
	}
	glFinish();
	}
	auto ntEnd = getNanoTime();

	checkOpenGLErrors();
	// Calculate SPH


	// TEMP
		// compute shader (change values)
		//m_TESTshader.runShader(1, 1, 1, true);

	//	-end- SHADERS
	////////////////////////////////////////////////

	
	long tEnd = getTime();
	// if enabled - log all particles positions into file
	if (LOG_TO_FILE) {
		ParticleData::logParticlePositions();
	}

	LOG_F(INFO, "Simulation time: %d, %f", tEnd - tStart, getNanoTimeDif(ntStart, ntEnd));
}

void Simulation::startSimulation(GLFWwindow* baseWindow)
{
	m_simulationWindow = glfwCreateWindow(10, 10, "OTHER", NULL, baseWindow);
	Simulation::m_simulationThread =  std::thread(Simulation::main);
	Threads::addThreadToList(&Simulation::m_simulationThread);
}

void Simulation::main()
{
	loguru::set_thread_name("Sim");
	LOG_F(INFO, "Simulation Thread START");
	glfwMakeContextCurrent(Simulation::m_simulationWindow);
	Simulation::init();
	ParticleData::initArraysOnGPU();

	Simulation::runSimulationFrame();
	ParticleObjectDetais details{ 1, 1,1,1, 1.1, 1.1, 10.1 };
	ParticleObjectDetais details2{ 2, 10,10,10, 10.1,10.1, 60 };
	ParticleObjectCreator::addObject(details);
	ParticleObjectCreator::addObject(details2);

	for(int i=0; i<100; i++) Simulation::runSimulationFrame();
	ParticleData::printSortingData();
	ParticleData::printParticleData(20);




	return;
	setupSimObjects();
	checkOpenGLErrors();

	while (!glfwWindowShouldClose(m_simulationWindow))
	{
		// run simulation 1 turn
		Simulation::runSimulationFrame();
		//ParticleData::printParticleData(2);
		//ParticleData::printGlassData(20);
		ParticleData::printSortingData();
	}
}

void Simulation::init()
{
	m_TESTshader = ComputeShader(ShaderFiles.TEST_ComputeShader);
	m_CellCounting = ComputeShader(ShaderFiles.CellCountingForSort);
	m_BitonicSort = ComputeShader(ShaderFiles.BitonicSort);
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


void setupSimObjects()
{
	ParticleObjectDetais details{ 1, 3,9,3, 22,10,7 };
	ParticleObjectCreator::addObject(details);

	Sleep(100);
	Simulation::runSimulationFrame();	// open resources
	Sleep(500);
	Simulation::runSimulationFrame();	// commit

	//ParticleObjectDetais details2{ -1, 5,4,5, 2.5,0,2.5 };
	//ParticleObjectCreator::addObject(details2);
	Sleep(100);
	Simulation::runSimulationFrame();	// open resources
	Sleep(500);
	Simulation::runSimulationFrame();	// commit
	Simulation::runSimulationFrame();
	Simulation::runSimulationFrame();
	//Sleep(100);
	ParticleData::printParticleData();
	ParticleData::printGlassData();
	ParticleObjectManager::printObjects();
	ParticleData::printParticleObjectsData();
}