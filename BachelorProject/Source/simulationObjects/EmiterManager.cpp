#include "EmiterManager.h"

Emiter* EmiterManager::createEmiter(int initNumberOfParticles, float initVelocity, int initFluidType)
{
	m_emitersVector.push_back(Emiter(initNumberOfParticles, initVelocity, initFluidType));
	return &m_emitersVector[m_emitersVector.size()-1];
}

void EmiterManager::printEmiters()
{
	LOG_F(INFO, "==============================");
	std::stringstream ss;
	for (Emiter& emiter : m_emitersVector) {
		ss << emiter.toString().c_str() << "\n";
	}
	LOG_F(INFO, "Emiters print\n%s", ss.str().c_str());
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

int EmiterManager::updateAllEmiters(int turnNumber)
{
	int emitedParticles = 0;
	ParticleData::openEmiters();
	GPUEmiter* emiterArray = ParticleData::m_resEmiters__MAP__;
	for (int i = 0; i < m_emitersVector.size(); i++) {
		emitedParticles += m_emitersVector[i].fillGPUdata(&(emiterArray[i]), turnNumber);
	}
	ParticleData::commitEmiters();
	return emitedParticles;
}













//void EmiterManager::setEmiter(EmiterProvider* matrixProvider, int numOfParticles, float velocity)
//{
//	//LOG_F(INFO, "Set emiter provider");
//	//m_matrixProvider = matrixProvider;
//	//m_controler->setNumOfEmitedParticles(numOfParticles);
//	//m_Velocity = min(velocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);
//	//if (m_emiterComputeShader != nullptr) m_emiterComputeShader->setUniformVariable(emiterVelocityUniform, m_Velocity);
//
//
//	m_emitFrequancy = int(ceil(Configuration.FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));
//	m_initState++;
//}
//
//void EmiterManager::setInputDispatcher(InputDispatcher* inputDispatcher)
//{
//	//m_inputDispatcher = inputDispatcher;
//	//m_controler->setInputDispatcher(inputDispatcher);
//	//m_initState++;
//}
//
//void EmiterManager::updateTurn(int turnNumber)
//{
//	////LOG_F(WARNING, "update turn %d", m_emitFrequancy);
//	//if (m_initState < m_TARGET_INIT_STATE) return;
//
//	//if (!m_controler->m_emiterActive) {
//	//	m_emiterComputeShader->setUniformVariable(emiterParticlesNumberUnifom, 0);
//	//	m_emitThisTurn = 0;
//	//	return;
//	//}
//
//	//if (m_controler->m_updateMatrix) {
//	//	m_Matrix = m_matrixProvider->getEmiterMatrix();
//	//}
//
//	//if (turnNumber % m_emitFrequancy == 0 && ParticleData::m_NumOfParticles + m_controler->m_numOfEmitedParticles < Configuration.MAX_PARTICLES) {
//	//	//LOG_F(WARNING, "%d / %d :: %s", m_emitFrequancy, turnNumber, glm::to_string(m_matrixProvider->getEmiterMatrix()).c_str());
//	//	m_emiterComputeShader->setUniformVariable(emiterMatrixUniform, m_Matrix);
//	//	m_emiterComputeShader->setUniformVariable(emiterParticlesNumberUnifom, m_controler->m_numOfEmitedParticles);
//	//	m_emitThisTurn = m_controler->m_numOfEmitedParticles;
//	//}
//	//else {
//	//	m_emiterComputeShader->setUniformVariable(emiterParticlesNumberUnifom, 0);
//	//	m_emitThisTurn = 0;
//	//}
//}
//
//void EmiterManager::setEmiterComputeShader(ComputeShader* shader)
//{
//	LOG_F(INFO, "Set emiter shader");
//	m_emiterComputeShader = shader;
//	if (m_Velocity != -1.0f) m_emiterComputeShader->setUniformVariable(emiterVelocityUniform, m_Velocity);
//	m_emiterComputeShader->setUniformVariable(emiterParticlesNumberUnifom, 0);
//	m_emitThisTurn = 0;
//	m_initState++;
//}
//
