#include "Simulation.h"

void setupSimObjects();

const std::string stageUniform = "u_stage";
const std::string turnUniform = "u_turnInStage";

void Simulation::runSimulationFrame()
{
	LOG_F(INFO, "LOOP");
	int dispathSize = -1;
	std::chrono::time_point<std::chrono::steady_clock>
		_ntStart, __ntEnd, _ntLoopStart, _ntParseRequests, _ntSynchronizeWithGpu, _ntCopyForSort, _ntCellCounting, _ntBitonicSort, _ntArrangeVars, _ntNeighbourSearch, _ntSyncDetails
		, _ntDensityPressureFluid, _ntAccelerationFluid, _ntVelocity;
	float _ntStartTime, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime, _ntNeighbourSearchTime, _ntSyncDetailsTime
		, _ntDensityPressureFluidTime, _ntAccelerationFluidTime, _ntVelocityTime;
	_ntSyncDetailsTime = _ntVelocityTime = _ntAccelerationFluidTime = _ntStartTime = _ntParseRequestsTime = _ntSynchronizeWithGpuTime = _ntCopyForSortTime = _ntCellCountingTime = _ntBitonicSortTime = _ntArrangeVarsTime = _ntNeighbourSearchTime = _ntDensityPressureFluidTime = 0;
	
	_ntStart = getNanoTime();
	for (int i = 0; i < 1000; i++) {
		//glFinish();
		_ntLoopStart = getNanoTime();
		while (ParticleObjectCreator::hasNewOrder()) {
			LOG_F(INFO, "Parsing Order loop");
			ParticleObjectCreator::createParticlesFromOrderList();
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_UNIFORM_BARRIER_BIT);
		}

		if (ParticleData::m_NumOfParticles == 0) continue;
		// open or close resources required by other threads
		if (ParticleData::m_OpenedResources > 0) {
			LOG_F(WARNING, "Simulation look with opened resources");
			return;
		}
		glFinish();		_ntParseRequests = getNanoTime();		_ntParseRequestsTime += getNanoTimeDif(_ntLoopStart, _ntParseRequests);

		// exchange information about glass objects with gpu
		ParticleObjectManager::synchronizeWithGpu();

		glFinish();		_ntSynchronizeWithGpu = getNanoTime();		_ntSynchronizeWithGpuTime += getNanoTimeDif(_ntParseRequests, _ntSynchronizeWithGpu);


		////////////////////////////////////////////////
		//	SHADERS 
		dispathSize = ceil(ParticleData::m_NumOfParticles / 256.0);

		// ASSIGN cells to particles
		m_CellCounting.runShader(dispathSize, 1, 1, false);

		glFinish(); _ntCellCounting = getNanoTime();		_ntCellCountingTime += getNanoTimeDif(_ntSynchronizeWithGpu, _ntCellCounting);

		ParticleData::copyDataForSorting();

		glFinish();		_ntCopyForSort = getNanoTime();		_ntCopyForSortTime += getNanoTimeDif(_ntCellCounting, _ntCopyForSort);

		// SORT
		const int bitonicSortWorkGroups = ceil(pow(2, ceil(log2(ParticleData::m_NumOfParticles))) / 256.0);	// min power of 2 more than num of particles threads / 256 threads per WorkGroup
		const int numOfStages = ceil(log2(ParticleData::m_NumOfParticles));
		for (int currentStage = 1; currentStage <= numOfStages; currentStage++) {
			for (int currentTurn = 1; currentTurn <= currentStage; currentTurn++) {
				m_BitonicSort.setUniformVariable(stageUniform, currentStage);
				m_BitonicSort.setUniformVariable(turnUniform, currentTurn);
				m_BitonicSort.runShader(dispathSize, 1, 1, false);
			}
		}
		glFinish();		_ntBitonicSort = getNanoTime();		_ntBitonicSortTime += getNanoTimeDif(_ntCopyForSort, _ntBitonicSort);

		// ARRANGE variables after sorting
		m_VariablesArrangement.runShader(dispathSize, 1, 1, false);
		glFinish();		_ntArrangeVars = getNanoTime();		_ntArrangeVarsTime += getNanoTimeDif(_ntBitonicSort, _ntArrangeVars);

		ParticleData::syncSimDetailsWithGpu();
		dispathSize = ceil(ParticleData::m_NumOfParticles / 256.0);

		glFinish();		_ntSyncDetails = getNanoTime();		_ntSyncDetailsTime += getNanoTimeDif(_ntArrangeVars, _ntSyncDetails);

		const int neighbourSearchDispatchSize = ceil(27.0 * ParticleData::m_NumOfParticles / 256.0);
		m_SphNeighbourSearch.runShader(neighbourSearchDispatchSize, 1, 1, false);

		glFinish();		_ntNeighbourSearch = getNanoTime();		_ntNeighbourSearchTime += getNanoTimeDif(_ntSyncDetails, _ntNeighbourSearch);

		m_SphDensityPressureFluid.runShader(dispathSize, 1, 1, false);

		glFinish();		_ntDensityPressureFluid = getNanoTime();		_ntDensityPressureFluidTime += getNanoTimeDif(_ntNeighbourSearch, _ntDensityPressureFluid);
		//ParticleData::printParticleData(20);
		m_SphAccelerationFluid.runShader(dispathSize, 1, 1, false);

		glFinish();		_ntAccelerationFluid = getNanoTime();		_ntAccelerationFluidTime += getNanoTimeDif(_ntDensityPressureFluid, _ntAccelerationFluid);

		m_SphVelocity.runShader(dispathSize, 1, 1, false);

		glFinish();		_ntVelocity = getNanoTime();		_ntVelocityTime += getNanoTimeDif(_ntAccelerationFluid, _ntVelocity);

		//m_TESTshader.runShader(1, 1, 1, true);	glFinish();

		if (LOG_TO_FILE) {
			ParticleData::logParticlePositions();
		}
		checkOpenGLErrors();
		
		//ParticleData::checkDensity();
	}
	__ntEnd = getNanoTime();
	//ParticleData::printParticleData(20);

	////////////////////////////////////////////////


	// if enabled - log all particles positions into file

	const float _ntSum = getNanoTimeDif(_ntStart, __ntEnd);

	LOG_F(INFO, "Simulation Particles: [%d, %d],dispathSize: %d time: %f \nRequests %f, SyncObj %f, SyncDet %f,  CopyBO %f, CellCounting %f, Sort %f, Arrange %f, Neighbours %f, Dens&PressFL %f, AccFL %f, Vel %f"
		, ParticleData::m_NumOfParticles, ParticleData::m_NumOfGlassParticles, dispathSize, _ntSum
		, _ntParseRequestsTime, _ntSynchronizeWithGpuTime, _ntSyncDetailsTime, _ntCopyForSortTime, _ntCellCountingTime, _ntBitonicSortTime, _ntArrangeVarsTime, _ntNeighbourSearchTime, _ntDensityPressureFluidTime, _ntAccelerationFluidTime, _ntVelocityTime);
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
	ParticleObjectDetais details{ 1, 10,10,10, 10.1, 10.1, 10.1};
	ParticleObjectDetais details2{ -1, 10,4,10, 5, 0, 0 };
	ParticleObjectDetais details3{ -1, 20,15,20, 15,1,10};
	ParticleObjectDetais detailsTEST{ 1, 15,20,15, 35, 40, 35 };
	ParticleObjectDetais detailsTESTGLASS{ -1, 25,10,25, 12,0.5,15 };
	//ParticleObjectCreator::addObject(details);
	//ParticleObjectCreator::addObject(details2);
	ParticleObjectCreator::addObject(detailsTEST);
	ParticleObjectCreator::addObject(detailsTESTGLASS);
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
	//for(int i=0; i<99999999999999; i++) 
	{
		// run simulation 1 turn
		Simulation::runSimulationFrame();
		//ParticleData::printGlassData(20);
	}
	/*ParticleData::printParticleData(20);
	ParticleData::printGlassParticlesData(20);
	ParticleData::printGlassObjectsData();
	ParticleData::printSortingData(20);
	ParticleData::printSPHData(1, 1, 1, 1, 1,10);
	ParticleData::printNeighboursData();*/
}

void Simulation::init()
{
	m_TESTshader = ComputeShader(ShaderFiles.TEST_ComputeShader);
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

