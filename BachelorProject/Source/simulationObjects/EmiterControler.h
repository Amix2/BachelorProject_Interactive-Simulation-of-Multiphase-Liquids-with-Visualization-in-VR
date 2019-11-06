#pragma once
#include <inputDispatcher/InputDispatcher.h>
#include <Logger.h>
#include <math.h>

class EmiterControler
	: public KeyInputListener
{
public:
	bool m_emiterActive = false;
	bool m_updateMatrix = true;
	int m_numOfEmitedParticles = 0;

	EmiterControler() {}
	void setInputDispatcher(InputDispatcher* inputDispatcher);
	void setNumOfEmitedParticles(int numOfParticles);

	void handleKeyPress(int key, KeyState action, float deltaTime);
};

