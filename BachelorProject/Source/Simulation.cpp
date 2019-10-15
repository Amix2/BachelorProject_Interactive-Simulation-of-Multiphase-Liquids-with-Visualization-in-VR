#include "Simulation.h"

#define TEST_TIME(value) if (MEASURE_TIME) { glFinish(); ##value += getNanoTimeDif(nTimeMain, nTimeTemp=getNanoTime()); nTimeMain=nTimeTemp;}

void setupSimObjects();

const std::string stageUniform = "u_stage";
const std::string turnUniform = "u_turnInStage";
bool test = true;
const bool MEASURE_TIME = true;

void Simulation::runSimulationFrame()
{
	LOG_F(INFO, "LOOP");
	std::chrono::time_point<std::chrono::steady_clock> nTimeTemp, nTimeMain;

	std::chrono::time_point<std::chrono::steady_clock> _ntStart, __ntEnd;

	float _ntStartTime, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime, _ntNeighbourSearchTime, _ntSyncDetailsTime
		, _ntDensityPressureFluidTime, _ntAccelerationFluidTime, _ntVelocityTime, _ntRangeCalc, _ntBufferClear;
	_ntBufferClear = _ntRangeCalc = _ntSyncDetailsTime = _ntVelocityTime = _ntAccelerationFluidTime = _ntStartTime = _ntParseRequestsTime = _ntSynchronizeWithGpuTime = _ntCopyForSortTime = _ntCellCountingTime = _ntBitonicSortTime = _ntArrangeVarsTime = _ntNeighbourSearchTime = _ntDensityPressureFluidTime = 0;
	
	_ntStart = getNanoTime();
	for (int i = 0; i < 1; i++) {
		//glFinish();
		nTimeMain = getNanoTime();
		while (ParticleObjectCreator::hasNewOrder()) {
			LOG_F(INFO, "Parsing Order loop");
			ParticleObjectCreator::createParticlesFromOrderList();
			//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_UNIFORM_BARRIER_BIT);
			ParticleData::printParticleData(0);
		}

		if (ParticleData::m_NumOfParticles == 0) continue;
		// open or close resources required by other threads
		if (ParticleData::m_OpenedResources > 0) {
			LOG_F(WARNING, "Simulation look with opened resources");
			return;
		}
		TEST_TIME(_ntParseRequestsTime);

		// exchange information about glass objects with gpu
		ParticleObjectManager::synchronizeWithGpu();

		TEST_TIME(_ntSynchronizeWithGpuTime);

		ParticleData::syncSimDetailsWithGpu();

		TEST_TIME(_ntSyncDetailsTime);

		const int numOfFluid = ParticleData::m_NumOfParticles;
		const int numOfFluidDiv256 = (int)ceil(numOfFluid / 256.0f);
		const int numOfFluidDivXX = (int)ceil(numOfFluid / 300.0f);
		const int numOfFluidDiv1024 = (int)ceil(numOfFluid / 1024.0f);
		const int numOfFluidPow2 = (int)pow(2, (int)ceil(log2(numOfFluid)));
		const int numOfFluidLog2 = (int)ceil(log2(numOfFluid));
		const int numOfFluidMul27Div256 = (int)ceil((27 * numOfFluid) / 256.0);
		const int numOfFluidMul27Div270 = (int)ceil((27 * numOfFluid) / 270.0);

		TEST_TIME(_ntRangeCalc);


		m_CellCounting.runShader(numOfFluidDiv256, 1, 1, false);


		TEST_TIME(_ntCellCountingTime);


		ParticleData::copyDataForSorting();


		TEST_TIME(_ntCopyForSortTime);


		// SORT
		const int bitonicSortWorkGroups = (int) ceil(numOfFluidPow2 * 0.5 / 256.0);
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


		m_SphAccelerationFluid.runShader(numOfFluidDiv256, 1, 1, false);


		TEST_TIME(_ntAccelerationFluidTime);


		m_SphVelocity.runShader(numOfFluidDiv256, 1, 1, false);


		TEST_TIME(_ntVelocityTime);



		//glClearNamedBufferData(GpuResources::getIndex(BufferDetails.NeighboursListName), GL_R32I, GL_R32I, GL_UNSIGNED_INT, NULL);


		//TEST_TIME(_ntBufferClear);


		// if enabled - log all particles positions into file
		if (LOG_TO_FILE) {ParticleData::logParticlePositions();}
		checkOpenGLErrors();
		
		//ParticleData::checkDensity();

	}
	__ntEnd = getNanoTime();

	////////////////////////////////////////////////


	const float _ntSum = getNanoTimeDif(_ntStart, __ntEnd);

	LOG_F(INFO, "Simulation Particles: [%d, %d] time: %f \nRequests %f, SyncObj %f, SyncDet %f, RCalc %f, CopyBO %f, CellCounting %f, Sort %f, Arrange %f, Neighbours %f, Dens&PressFL %f, AccFL %f, Vel %f, Cls %f"
		, ParticleData::m_NumOfParticles, ParticleData::m_NumOfGlassParticles, _ntSum
		, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntSyncDetailsTime, _ntRangeCalc, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime, _ntNeighbourSearchTime, _ntDensityPressureFluidTime, _ntAccelerationFluidTime, _ntVelocityTime, _ntBufferClear);

	LOG_F(INFO, "Simulation Particles: [%d, %d] \n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n"
		, ParticleData::m_NumOfParticles, ParticleData::m_NumOfGlassParticles, _ntSum
		, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntSyncDetailsTime, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime, _ntNeighbourSearchTime, _ntDensityPressureFluidTime, _ntAccelerationFluidTime, _ntVelocityTime);

}

void Simulation::startSimulation(GLFWwindow* baseWindow)
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_simulationWindow = glfwCreateWindow(10, 10, "OTHER", NULL, baseWindow);
	m_mainWindow = baseWindow;
	Simulation::m_simulationThread = std::thread(Simulation::main);
	Threads::addThreadToList(&Simulation::m_simulationThread);
}

void setupSimObjects()
{
	ParticleObjectDetais details{ 1, 10.1,4.5,10.1, 10.2, 4.8, 10.2};
	ParticleObjectDetais details2{ -1, 10,4,10, 5, 0, 0 };
	ParticleObjectDetais details3{ -1, 20,15,20, 15,1,10};
	ParticleObjectDetais detailsTEST{ 1, 20,20,20, 40, 80, 40 };
	ParticleObjectDetais detailsTESTGLASS{ -1, 30,25,30, 20.5,0.5,15 };
	ParticleObjectDetais optimFluid{ 1, 20,20,20, 60, 50, 60 };
	ParticleObjectDetais optimGlass{ -1, 40,35,40, 28,0.5,35 };
	ParticleObjectCreator::addObject(details);
	ParticleObjectCreator::addObject(details2);
	//ParticleObjectCreator::addObject(detailsTEST);
	//ParticleObjectCreator::addObject(detailsTESTGLASS);
	//ParticleObjectCreator::addObject(optimFluid);
	//ParticleObjectCreator::addObject(optimGlass);
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

	//while (!glfwWindowShouldClose(m_mainWindow))
	for(int i=0; i<1; i++) 
	{
		// run simulation 1 turn
		Simulation::runSimulationFrame();
		//ParticleData::printGlassData(20);
	}
	//ParticleData::printParticleData(20);
	//ParticleData::printGlassParticlesData(20);
	//ParticleData::printGlassObjectsData();
	//ParticleData::printSortingData(20);
	ParticleData::printSPHData(1, 1, 1, 1, 1,220);
	//ParticleData::printNeighboursData();
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

}
//
//void Simulation::parseResourceRequest()
//{
//
//	switch (m_reqFluidArray) {
//	case OPEN:
//		ParticleData::__OLD__openFluidArray();
//		m_reqFluidArray = NO_ORDER;
//		break;
//	case COMMIT:
//		ParticleData::__OLD__commitFluidArray();
//		m_reqFluidArray = NO_ORDER;
//		break;
//	case NO_ORDER:
//		break;
//	}
//
//	switch (m_reqGlassArray) {
//	case OPEN:
//		ParticleData::__OLD__openGlassArray();
//		m_reqGlassArray = NO_ORDER;
//		break;
//	case COMMIT:
//		ParticleData::__OLD__commitGlassArray();
//		m_reqGlassArray = NO_ORDER;
//		break;
//	case NO_ORDER:
//		break;
//	}
//
//	switch (m_reqGlassVectorsArray) {
//	case OPEN:
//		ParticleData::openGlassVectors();
//		m_reqGlassVectorsArray = NO_ORDER;
//		break;
//	case COMMIT:
//		ParticleData::commitGlassVectors();
//		m_reqGlassVectorsArray = NO_ORDER;
//		break;
//	case NO_ORDER:
//		break;
//	}
//
//	switch (m_reqDetils) {
//	case OPEN:
//		ParticleData::openDetails();
//		m_reqDetils = NO_ORDER;
//		break;
//	case COMMIT:
//		ParticleData::commitDetails();
//		m_reqDetils = NO_ORDER;
//		break;
//	case NO_ORDER:
//		break;
//	}
//
//	switch (m_reqObjects) {
//	case OPEN:
//		ParticleData::openObjects();
//		m_reqObjects = NO_ORDER;
//		break;
//	case COMMIT:
//		ParticleData::commitObjects();
//		m_reqObjects = NO_ORDER;
//		break;
//	case COMMIT_AND_OPEN:
//		ParticleData::commitObjects();
//		ParticleData::openObjects();
//		m_reqObjects = NO_ORDER;
//	case NO_ORDER:
//		break;
//	}
//
//	checkOpenGLErrors();
//}

