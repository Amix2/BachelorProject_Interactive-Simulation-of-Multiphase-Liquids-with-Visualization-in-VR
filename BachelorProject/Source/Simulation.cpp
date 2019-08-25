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
	const bool canOpenNew = (ParticleData::m_OpenResource == NONE);

	switch (m_ResourceRequest) {
	case OPEN_FLUID:
		if (!canOpenNew) {
			// other resource in open, cant open new one
			break;
		}
		ParticleData::openToAddFluidArray();
		break;
	case OPEN_GLASS:
		if (!canOpenNew) {
			// other resource in open, cant open new one
			break;
		}
		ParticleData::openToAddGlassArray();
		break;
	case COMMIT:
		if (canOpenNew) {
			// no open resource
			throw "No open resource, can't commit";
		}
		ParticleData::commitToAddArray();
		m_ResourceRequest = NO_ORDER;
		break;
	case NO_ORDER:
		break;
	}
}

Simulation::Simulation()
{
	shader = ComputeShader("./Source/shaders/compute.glsl");
}
