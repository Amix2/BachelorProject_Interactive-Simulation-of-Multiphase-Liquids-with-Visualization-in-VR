#include "Simulation.h"


void Simulation::runSimulation()
{
	loguru::set_thread_name("simulation");
	LOG_F(INFO, "simulation running");

	parseResourceRequest();

	// compute shader (change values)
	long tStart = getTime();
	shader.runShader(1000, 1, 1, true);
	long tEnd = getTime();

	
	LOG_F(INFO, "Simulation time: %d", tEnd - tStart);

	checkOpenGLErrors();
}

void Simulation::parseResourceRequest()
{

	switch (m_reqFluidArray) {
	case OPEN:
		ParticleData::openFluidArray();
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
		break;
	case COMMIT:
		ParticleData::commitGlassArray();
		m_reqGlassArray = NO_ORDER;
		break;
	case NO_ORDER:
		break;
	}

	switch (m_reqDetils) {
	case OPEN:
		ParticleData::openDetails();
		break;
	case COMMIT:
		ParticleData::commitDetails();
		m_reqDetils = NO_ORDER;
		break;
	case NO_ORDER:
		break;
	}
}

Simulation::Simulation()
{
	shader = ComputeShader("./Source/shaders/compute.glsl");
}
