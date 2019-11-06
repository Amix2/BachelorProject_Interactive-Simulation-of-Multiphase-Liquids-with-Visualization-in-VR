#include "EmiterControler.h"
void EmiterControler::setInputDispatcher(InputDispatcher* inputDispatcher)
{
	inputDispatcher->subscribeForKeyInput(this, std::vector<int>{
		GLFW_KEY_0,
		GLFW_KEY_P,
		GLFW_KEY_EQUAL,
		GLFW_KEY_MINUS
	});
}
void EmiterControler::setNumOfEmitedParticles(int numOfParticles)
{
	m_numOfEmitedParticles = numOfParticles;
}
void EmiterControler::handleKeyPress(int key, KeyState action, float deltaTime)
{
	if (action == KeyState::FALLING_EDGE) {
		switch (key) {
		case GLFW_KEY_0:
			m_emiterActive = !m_emiterActive;
			break;
		case GLFW_KEY_P:
			m_updateMatrix = !m_updateMatrix;
			break;
		case GLFW_KEY_EQUAL:
			m_numOfEmitedParticles += int(ceil(sqrt(m_numOfEmitedParticles)));
			break;
		case GLFW_KEY_MINUS:
			m_numOfEmitedParticles -= int(floor(sqrt(m_numOfEmitedParticles)));
			break;
		default:
			break;
		}
	}
}