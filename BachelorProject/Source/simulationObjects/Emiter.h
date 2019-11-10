#pragma once
#include <glm/glm.hpp>
#include <Configuration.h>
#include <sstream>
#include <simulationObjects/EmiterProvider.h>
#include <glm/gtx/string_cast.hpp>
#include <dataStructures/ParticleData.h>

struct GPUEmiter {
	glm::mat4 matrix;
	float velocity;
	int emitThisTurn;
	int fluidType;
	float rotationAngle;
};

class Emiter
{
private:
	EmiterProvider* m_provider	= nullptr;
	glm::mat4 m_Matrix	= glm::mat4();
	int m_emitFrequency	= 0;
	float m_Velocity	= 0;
	int m_emitThisTurn	= 0;
	int m_numOfParticles	= 0;
	int m_fluidType = 0;
	bool m_isActive		= true;
	bool m_updateMatrix = true;
	float m_rotationAngle	= 0;
public:
	Emiter(EmiterProvider* provider, int initNumberOfParticles, float initVelocity, int initFluidType);
	Emiter() {}

	int fillGPUdata(GPUEmiter* data, int turnNumber);

	std::string toString();


	int changeSize(int rowsNumber);
	int increaseSize(int addRows);
	int decreaseSize(int revemeRows);
	void setActive(bool active) { m_isActive = active; }
	void toggleActive() { m_isActive = !m_isActive; }
	void setMatrixUpdate(bool update) { m_updateMatrix = update; }
	void togleMatrixUpdate() { m_updateMatrix = !m_updateMatrix; }
};

