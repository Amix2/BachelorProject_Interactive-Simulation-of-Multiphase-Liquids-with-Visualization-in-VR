#include "Emiter.h"

Emiter::Emiter(int initNumberOfParticles, float initVelocity, int initFluidType) : m_numOfParticles(initNumberOfParticles), m_fluidType(initFluidType)
{
	m_Velocity = min(initVelocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);

	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));

	//m_Matrix = glm::mat4();
}

glm::mat4 rotationMatrix(glm::vec3 axis, float angle)
{
	const float s = sin(angle);
	const float c = cos(angle);
	const float oc = 1.0 - c;

	return glm::mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
		0.0, 0.0, 0.0, 1.0);
}

int Emiter::fillGPUdata(GPUEmiter* data, int turnNumber)
{
	if (!m_isActive) {
		data->emitThisTurn = 0;
		m_emitThisTurn = 0;
		return 0;
	}

	m_rotationAngle += 0.1;
	if (m_updateMatrix) {
		//m_Matrix = m_provider->getEmiterMatrix();
	}

	data->matrix = m_Matrix * glm::rotate(glm::mat4{ 1 }, m_rotationAngle, glm::vec3(0,0,1));
	data->velocity = m_Velocity;
	data->fluidType = m_fluidType;
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

void Emiter::updateMatrix(const glm::mat4& matrix)
{
	if (m_updateMatrix) {
		m_Matrix = matrix;
	}
}


int Emiter::changeSize(int rowsNumber)
{
	m_numOfParticles += rowsNumber;
	if (m_numOfParticles <= 0) m_numOfParticles = 1;
	return m_numOfParticles;
}

int Emiter::increaseSize(int addRows)
{
	m_numOfParticles += addRows;
	return m_numOfParticles;
}

int Emiter::decreaseSize(int revemeRows)
{
	m_numOfParticles -= revemeRows;
	if (m_numOfParticles <= 0) m_numOfParticles = 1;
	return m_numOfParticles;
}

float Emiter::increaseVelocity(int stepsUp)
{
	for(int i=stepsUp; i>0; i--)
		m_Velocity *= 1.1f;
	m_Velocity = min(m_Velocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);
	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));
	return m_Velocity;
}

float Emiter::decreaseVelocity(int stepsDown)
{
	for (int i = stepsDown; i > 0; i--) {
		m_Velocity *= 0.9f;
		if (m_Velocity < 1.0f) m_Velocity = 1.0f;
	}
	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));
	return m_Velocity;
}

float Emiter::chengeVeloccity(float defVelocity)
{
	m_Velocity += defVelocity;
	m_Velocity = min(m_Velocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);
	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));
	return m_Velocity;
}
