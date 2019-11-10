#include "Emiter.h"

Emiter::Emiter(EmiterProvider* provider, int initNumberOfParticles, float initVelocity, int initFluidType) : m_provider(provider), m_numOfParticles(initNumberOfParticles), m_fluidType(initFluidType)
{
	m_Velocity = min(initVelocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);

	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));

	//m_Matrix = glm::mat4();
}

int Emiter::fillGPUdata(GPUEmiter* data, int turnNumber)
{
	if (!m_isActive) {
		m_emitThisTurn = 0;
		return 0;
	}

	m_rotationAngle += 0.1;
	if (m_updateMatrix) {
		m_Matrix = m_provider->getEmiterMatrix();
	}
	data->matrix = m_Matrix;
	data->velocity = m_Velocity;
	data->fluidType = m_fluidType;
	data->rotationAngle = m_rotationAngle;
	if (turnNumber % m_emitFrequency == 0 && ParticleData::m_NumOfParticles + 512 + m_numOfParticles < Configuration.MAX_PARTICLES) {
		m_emitThisTurn = m_numOfParticles;
	}
	else {
		m_emitThisTurn = 0;
	}
	data->emitThisTurn = m_emitThisTurn;
	return m_emitThisTurn * m_emitThisTurn;
}

std::string Emiter::toString()
{
	std::stringstream ss;
	ss << "Emiter :: active: \t" << (m_isActive ? "true" : "false") << "\n\tParticles: " << m_numOfParticles << ", this turn: " << m_emitThisTurn
		<< "\n\tVel: " << m_Velocity
		<< "\n\tFrequency: " << m_emitFrequency
		<< "\n\tMat ["<< (m_updateMatrix ? "update" : "no-update") <<"]: " << glm::to_string(m_Matrix);
	return std::string(ss.str());
}


int Emiter::changeSize(int rowsNumber)
{
	LOG_F(WARNING, "changeSize, %d", rowsNumber);
	m_numOfParticles += rowsNumber;
	if (m_numOfParticles <= 0) m_numOfParticles = 1;
	return m_numOfParticles;
}

int Emiter::increaseSize(int addRows)
{
	LOG_F(WARNING, "increaseSize, %d", addRows);
	m_numOfParticles += addRows;
	return m_numOfParticles;
}

int Emiter::decreaseSize(int revemeRows)
{
	LOG_F(WARNING, "decreaseSize, %d", revemeRows);
	m_numOfParticles -= revemeRows;
	if (m_numOfParticles <= 0) m_numOfParticles = 1;
	return m_numOfParticles;
}
