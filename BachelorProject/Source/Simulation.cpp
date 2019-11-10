#include "Simulation.h"

float TimeTemp, TimeMain;
#define TEST_TIME(value) if (MEASURE_TIME) { glFinish();  TimeTemp=(float)glfwGetTime(); ##value += TimeTemp - TimeMain; TimeMain=TimeTemp;}

void Simulation::runSimulationFrame()
{
	//LOG_F(INFO, " =====");
	if (MEASURE_TIME)TimeMain = glfwGetTime();
	while (ParticleObjectCreator::hasNewOrder()) {
		LOG_F(INFO, "Parsing Order loop");
		ParticleObjectCreator::createParticlesFromOrderList();
		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_UNIFORM_BARRIER_BIT);
		//ParticleData::printParticleData(0);
		//ParticleData::syncSimDetailsWithGpu();
		//glFinish();
	}

	//if (ParticleData::m_NumOfParticles + Simulation::m_emiterNumOfParticles == 0) return;
	// open or close resources required by other threads
	if (ParticleData::m_OpenedResources > 0) {
		LOG_F(WARNING, "Simulation loop with opened resources");
		return;
	}
	TEST_TIME(_ntParseRequestsTime);

	// exchange information about glass objects with gpu
	ParticleObjectManager::synchronizeWithGpu();
	const int emitedThisTurn = EmiterManager::updateAllEmiters(m_turnNumber);

	TEST_TIME(_ntSynchronizeWithGpuTime);

	ParticleData::syncSimDetailsWithGpu();

	TEST_TIME(_ntSyncDetailsTime);

	const int numOfFluid = ParticleData::m_NumOfParticles + emitedThisTurn;
	const int numOfFluidDiv256 = (int)ceil(numOfFluid / 256.0f);
	const int numOfFluidDivXX = (int)ceil(numOfFluid / 300.0f);
	const int numOfFluidDiv1024 = (int)ceil(numOfFluid / 1024.0f);
	const int numOfFluidPow2 = (int)pow(2, (int)ceil(log2(numOfFluid)));
	const int numOfFluidLog2 = (int)ceil(log2(numOfFluid));
	const int numOfFluidMul27Div256 = (int)ceil((27 * numOfFluid) / 256.0);
	const int numOfFluidMul27Div270 = (int)ceil((27 * numOfFluid) / 270.0);

	TEST_TIME(_ntRangeCalc);


	m_CellCounting.runShader(numOfFluidDiv256, 1, 1, false);
	ParticleData::m_NumOfParticles += emitedThisTurn;
	ParticleData::syncSimDetailsWithGpu(emitedThisTurn);
	//return;
	TEST_TIME(_ntCellCountingTime);


	ParticleData::copyDataForSorting();

	TEST_TIME(_ntCopyForSortTime);


	// SORT
	const int bitonicSortWorkGroups = (int)ceil(numOfFluidPow2 * 0.5 / 256.0);
	const int numOfStages = numOfFluidLog2;
	for (int currentStage = 1; currentStage <= numOfStages; currentStage++) {
		m_BitonicSort.setUniformVariable(stageUniform, currentStage);
		for (int currentTurn = 1; currentTurn <= currentStage; currentTurn++) {
			m_BitonicSort.setUniformVariable(turnUniform, currentTurn);
			m_BitonicSort.runShader(bitonicSortWorkGroups, 1, 1, false);
		}
	}


	TEST_TIME(_ntBitonicSortTime);


	// ARRANGE variables after sorting
	m_VariablesArrangement.runShader(numOfFluidDiv256, 1, 1, false);


	TEST_TIME(_ntArrangeVarsTime);

	m_SphNeighbourSearch.runShader(numOfFluidMul27Div256, 1, 1, false);


	TEST_TIME(_ntNeighbourSearchTime);


	m_SphDensityPressureFluid.runShader(numOfFluidMul27Div270, 1, 1, false);


	TEST_TIME(_ntDensityPressureFluidTime);


	m_SphAccelerationFluid.runShader(numOfFluidMul27Div270, 1, 1, false);


	TEST_TIME(_ntAccelerationFluidTime);


	m_SphVelocity.runShader(numOfFluidDiv256, 1, 1, false);


	TEST_TIME(_ntVelocityTime);
	ParticleData::syncSimDetailsWithGpu(0);

	checkOpenGLErrors();

}

void Simulation::startSimulation(GLFWwindow* baseWindow)
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_simulationWindow = glfwCreateWindow(10, 10, "OTHER", NULL, baseWindow);
	m_mainWindow = baseWindow;
	Simulation::m_simulationThread = std::thread(Simulation::main);
	Threads::addThreadToList(&Simulation::m_simulationThread);
}

void Simulation::setupSimObjects()
{
	ParticleObjectDetais details{ 1, 10.1,4.5,10.1, 10.2, 14.8, 10.2};
	ParticleObjectDetais details2{ -1, 10,4,10, 2, 0, 0 };

	ParticleObjectDetais details3{ -1, 20,13,20, 3,0,0};

	ParticleObjectDetais details4{ -1, 20,5,20, 3,0.5,0 };

	ParticleObjectDetais details5{ 1, 20, 15.14, 20, 20.1,15.3,20.1 };

	ParticleObjectDetais details6{ 1, 20, 5.1, 20, 20.1,5.3,20.1 };

	ParticleObjectDetais detailsTEST{ 1, 25, 30,25, 35, 70, 35 };
	ParticleObjectDetais detailsTESTGLASS{ -1, 30,25,30, 20.5,0.5,15 };

	ParticleObjectDetais optimFluid{ 1, 121,70,121, 159, 100, 159 };
	ParticleObjectDetais optimGlass{ -1, 140,55,140, 28,0.5,35 };
	const float x = 40, y = 27.7, z = 11.80;
	ParticleObjectDetais detailsSing{ 1, x, y, z, x+0.1, y+0.1, z+0.1};

	ParticleObjectDetais detailsSing2{ 1, x+0.05, y + 0.05, z + 0.05, x + 0.1 + 0.05, y + 0.1 + 0.05, z + 0.1 + 0.05 };

	//ParticleObjectCreator::addObject(details);
	//ParticleObjectCreator::addObject(details2);

	//ParticleObjectCreator::addObject(details6);
	//ParticleObjectCreator::addObject(details5);

	//ParticleObjectCreator::addObject(details4);
	//ParticleObjectCreator::addObject(details3);
	
	//ParticleObjectCreator::addObject(detailsTEST);
	//ParticleObjectCreator::addObject(detailsTESTGLASS);
	//ParticleObjectCreator::addObject(optimFluid);
	//ParticleObjectCreator::addObject(optimGlass);
	//ParticleObjectCreator::addObject(detailsSing);
	//ParticleObjectCreator::addObject(detailsSing2);

	//Simulation::setEmiterMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(10, 30, 20)));
	//Simulation::setEmiterNumOfParticles(9);
	//Simulation::setEmiterVelocity(100);
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

	double timeStart, timeEnd, timeDif = 0;
	while (!glfwWindowShouldClose(m_mainWindow))
	for (int i = 0; i < 1; i++)
	{
		m_turnNumber++;
		timeStart = glfwGetTime();


		//EmiterManager::updateTurn(m_turnNumber);
		Simulation::runSimulationFrame();
		//glFinish();
		Simulation::handlePrintingTimes();


		timeEnd = glfwGetTime();

		const int sleepTime = (1.0 / 30 - (timeEnd - timeStart)) * 1000;
		if(sleepTime > 0)
			Sleep(sleepTime);

		timeDif += timeEnd - timeStart;

		if (m_turnNumber % m_forTitleTimesFrequency == 0) {
			WindowTitle::setTitle(m_SimFpsTitle, "Sim FPS [max " + std::to_string(Configuration.TARGET_SIM_FPS) + "]: " + std::to_string((int)(m_forTitleTimesFrequency / timeDif)));
			WindowTitle::setTitle(m_SimParticleTitle, std::to_string(ParticleData::m_NumOfParticles) +" particles");
			WindowTitle::setTitle(m_SimGlassParticleTitle, std::to_string(ParticleData::m_NumOfGlassParticles) + " glass particles");
			timeDif = 0;
		}

		EmiterManager::printEmiters();
		//ParticleData::printGlassObjectsData(2);
		//ParticleData::printParticleData(2000);
		//Sleep(5000);
		//ParticleData::printSPHData(1, 1, 1, 1, 1, 2000);

	}
	//ParticleData::printNeighboursData(1);
	//ParticleData::printParticleData(200000);
	//ParticleData::printGlassParticlesData(2000);
	//ParticleData::printGlassObjectsData(2);
	//ParticleObjectManager::printObjects(2);
	//ParticleData::printSPHData(1, 1, 1, 1, 1,200000);
	//ParticleData::printSortingData(20);
}

void Simulation::init()
{
	//m_TESTshader = ComputeShader(ShaderFiles.TEST_ComputeShader);
	m_CellCounting = ComputeShader(ShaderFiles.CellCountingForSort);
	m_BitonicSort = ComputeShader(ShaderFiles.BitonicSort);
	m_VariablesArrangement = ComputeShader(ShaderFiles.VariablesArrangementAfterSort);
	m_SphNeighbourSearch = ComputeShader(ShaderFiles.SphNeighbourSearch);
	m_SphDensityPressureFluid = ComputeShader(ShaderFiles.SphDensityPressureFluid);
	m_SphAccelerationFluid = ComputeShader(ShaderFiles.SphAccelerationFluid);
	m_SphVelocity = ComputeShader(ShaderFiles.SphVelocity);

	WindowTitle::addTitle(m_SimFpsTitle, 3);
	WindowTitle::addTitle(m_SimParticleTitle, 4);
	WindowTitle::addTitle(m_SimGlassParticleTitle, 5);

	//EmiterManager::setEmiterComputeShader(&m_CellCounting);
}

void Simulation::handlePrintingTimes()
{
	if (!PRINT_TIME || m_turnNumber % m_printTimesFrequency != 0) return;
	if (MEASURE_TIME) {

		_ntSum = _ntRangeCalc + _ntSyncDetailsTime + _ntVelocityTime + _ntAccelerationFluidTime + _ntParseRequestsTime + _ntSynchronizeWithGpuTime + _ntCopyForSortTime + _ntCellCountingTime + _ntBitonicSortTime + _ntArrangeVarsTime + _ntNeighbourSearchTime + _ntDensityPressureFluidTime;

		LOG_F(INFO, "Simulation Turn: %u, Particles: [%d, %d], Time: %0.2f \nRequests %0.2f, SyncObj %0.2f, SyncDet %0.2f, RCalc %0.2f, CopyBO %0.2f, CellCounting %0.2f, Sort %0.2f, Arrange %0.2f, Neighbours %0.2f, Dens&PressFL %0.2f, AccFL %0.2f, Vel %0.2f"
			, m_turnNumber, ParticleData::m_NumOfParticles, ParticleData::m_NumOfGlassParticles, _ntSum
			, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntSyncDetailsTime, _ntRangeCalc, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime, _ntNeighbourSearchTime, _ntDensityPressureFluidTime, _ntAccelerationFluidTime, _ntVelocityTime);

		LOG_F(INFO, "Simulation Particles: [%d, %d] \n%0.2f\n%0.2f\n%0.2f\n%0.2f\n%0.2f\n%0.2f\n%0.2f\n%0.2f\n%0.2f\n%0.2f\n%0.2f\n%0.2f\n"
			, ParticleData::m_NumOfParticles, ParticleData::m_NumOfGlassParticles, _ntSum
			, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntSyncDetailsTime, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime, _ntNeighbourSearchTime, _ntDensityPressureFluidTime, _ntAccelerationFluidTime, _ntVelocityTime);

		_ntRangeCalc = _ntSyncDetailsTime = _ntVelocityTime = _ntAccelerationFluidTime = _ntParseRequestsTime = _ntSynchronizeWithGpuTime = _ntCopyForSortTime = _ntCellCountingTime = _ntBitonicSortTime = _ntArrangeVarsTime = _ntNeighbourSearchTime = _ntDensityPressureFluidTime = 0;
	
	} else {

		LOG_F(INFO, "Simulation Turn: %u, Particles: [%d, %d], Time: %0.2f", m_turnNumber, ParticleData::m_NumOfParticles, ParticleData::m_NumOfGlassParticles, _ntSum/m_printTimesFrequency);
		_ntSum = 0;

	}
}