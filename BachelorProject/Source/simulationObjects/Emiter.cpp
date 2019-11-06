#include "Emiter.h"

void Emiter::setEmiter(EmiterMatrixProvider* matrixProvider, int numOfParticles, float velocity)
{
	LOG_F(INFO, "Set emiter provider");
	m_matrixProvider = matrixProvider;
	m_controler->setNumOfEmitedParticles(numOfParticles);
	m_emiterVelocity = min(velocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);
	if (m_emiterComputeShader != nullptr) m_emiterComputeShader->setUniformVariable(emiterVelocityUniform, m_emiterVelocity);


	m_emitFrequancy = int(ceil(Configuration.FLUID_PARTICLE_BUILD_GAP / (m_emiterVelocity*Configuration.DELTA_TIME)));
	m_initState++;
}

void Emiter::setInputDispatcher(InputDispatcher* inputDispatcher)
{
	m_inputDispatcher = inputDispatcher;
	m_controler->setInputDispatcher(inputDispatcher);
	m_initState++;
}

void Emiter::updateTurn(int turnNumber)
{
	//LOG_F(WARNING, "update turn %d", m_emitFrequancy);
	if (m_initState < m_TARGET_INIT_STATE) return;

	if (!m_controler->m_emiterActive) {
		m_emiterComputeShader->setUniformVariable(emiterParticlesNumberUnifom, 0);
		m_emiterdThisTurn = 0;
		return;
	}

	if (m_controler->m_updateMatrix) {
		m_emiterMatrix = m_matrixProvider->getEmiterMatrix();
	}

	if (turnNumber % m_emitFrequancy == 0  &&  ParticleData::m_NumOfParticles + m_controler->m_numOfEmitedParticles < Configuration.MAX_PARTICLES) {
		//LOG_F(WARNING, "%d / %d :: %s", m_emitFrequancy, turnNumber, glm::to_string(m_matrixProvider->getEmiterMatrix()).c_str());
		m_emiterComputeShader->setUniformVariable(emiterMatrixUniform, m_emiterMatrix);
		m_emiterComputeShader->setUniformVariable(emiterParticlesNumberUnifom, m_controler->m_numOfEmitedParticles);
		m_emiterdThisTurn = m_controler->m_numOfEmitedParticles;
	}
	else {
		m_emiterComputeShader->setUniformVariable(emiterParticlesNumberUnifom, 0);
		m_emiterdThisTurn = 0;
	}
}

void Emiter::setEmiterComputeShader(ComputeShader* shader)
{
	LOG_F(INFO, "Set emiter shader");
	m_emiterComputeShader = shader;
	if(m_emiterVelocity != -1.0f) m_emiterComputeShader->setUniformVariable(emiterVelocityUniform, m_emiterVelocity);
	m_emiterComputeShader->setUniformVariable(emiterParticlesNumberUnifom, 0);
	m_emiterdThisTurn = 0;
	m_initState++;
}

