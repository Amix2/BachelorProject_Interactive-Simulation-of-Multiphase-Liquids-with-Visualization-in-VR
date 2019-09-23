#include "Simulation.h"

void setupSimObjects();

void Simulation::runSimulationFrame()
{
	long tStart = getTime();

	// open or close resources required by other threads
	parseResourceRequest();

	// exchange information about glass objects with gpu
	ParticleObjectManager::synchronizeWithGpu();

	////////////////////////////////////////////////
	//	SHADERS 

	// Move glass particles


	// Sort fluid particle array


	// Calculate SPH


	// TEMP
		// compute shader (change values)
		m_TESTshader.runShader(1, 1, 1, true);

	//	-end- SHADERS
	////////////////////////////////////////////////

	long tEnd = getTime();
	
	// if enabled - log all particles positions into file
	if (LOG_TO_FILE) {
		ParticleData::logParticlePositions();
	}

	checkOpenGLErrors();
	LOG_F(INFO, "Simulation time: %d", tEnd - tStart);
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

	setupSimObjects();
	checkOpenGLErrors();

	while (!glfwWindowShouldClose(m_simulationWindow))
	{
		// run simulation 1 turn
		Simulation::runSimulationFrame();
		//ParticleData::printParticleData();
	}
}

void Simulation::init()
{
	m_TESTshader = ComputeShader(ShaderFiles.TEST_ComputeShader);
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
	ParticleObjectCreator::canAddObject();
	ParticleObjectDetais details{ 1, 3,9,3, 7,10,7 };
	ParticleObjectCreator::addObject(details);

	Sleep(10);
	Simulation::runSimulationFrame();	// open resources
	Sleep(500);
	Simulation::runSimulationFrame();	// commit

	ParticleObjectDetais details2{ -1, 5,4,5, 2.5,0,2.5 };
	ParticleObjectCreator::addObject(details2);
	Sleep(10);
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