#include "Simulation.h"


void Simulation::runSimulation()
{
	//LOG_F(INFO, "simulation running, orders: Particle %d, Glass %d, glVectors %d, details %d, objects %d", (int)m_reqFluidArray, (int)m_reqGlassArray, (int)m_reqGlassVectorsArray, (int)m_reqDetils, (int)m_reqObjects);

	parseResourceRequest();
	long tStart = getTime();

	ParticleObjectManager::synchronizeWithGpu();

	// compute shader (change values)
	shader.runShader(1, 1, 1, true);

	long tEnd = getTime();
	
	if (LOG_TO_FILE) {
		ParticleData::logParticlePositions();
	}
	checkOpenGLErrors();
	LOG_F(INFO, "Simulation time: %d", tEnd - tStart);
}

void Simulation::init()
{
	shader = ComputeShader("./Source/shaders/compute.glsl");
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
		ParticleData::openPartVectors();
		m_reqObjects = NO_ORDER;
		break;
	case COMMIT:
		ParticleData::commitPartVectors();
		m_reqObjects = NO_ORDER;
		break;
	case COMMIT_AND_OPEN:
		ParticleData::commitPartVectors();
		ParticleData::openPartVectors();
		m_reqObjects = NO_ORDER;
	case NO_ORDER:
		break;
	}

	checkOpenGLErrors();
}
