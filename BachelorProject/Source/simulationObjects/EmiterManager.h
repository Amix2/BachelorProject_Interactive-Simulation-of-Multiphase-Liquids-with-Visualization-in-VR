#pragma once

#include <simulationObjects/EmiterProvider.h>
#include <Configuration.h>
#include <Logger.h>
#include <glm/gtx/string_cast.hpp>
#include <simulationObjects/Emiter.h>
#include <memory>
#include <Logger.h>

class EmiterManager
{
private:
	inline static int m_initState = 0;
	inline static int m_TARGET_INIT_STATE = 3;

	inline static std::vector<Emiter> m_emitersVector;

public:

	static Emiter* createEmiter(EmiterProvider* provider, int initNumberOfParticles, float initVelocity, int initFluidType);

	static void printEmiters();

	static int updateAllEmiters(int turnNumber);



	//inline static ComputeShader* m_emiterComputeShader = nullptr;
	//inline static EmiterProvider* m_matrixProvider;
	//inline static InputDispatcher* m_inputDispatcher;

	//inline static glm::mat4 m_Matrix;
	//inline static int m_emitFrequancy;
	//inline static float m_Velocity = -1.0f;
	//inline static int m_emitThisTurn = 0;

	//static void setEmiter(EmiterProvider* matrixProvider, int numOfParticles, float velocity);
	//static void setInputDispatcher(InputDispatcher* inputDispatcher);

	//static void updateTurn(int turnNumber);

	//static void setEmiterComputeShader(ComputeShader* shader);

	//inline static const std::string emiterMatrixUniform = "u_emiterMatrix";
	//inline static const std::string emiterParticlesNumberUnifom = "u_emiterParticlesNumber";
	//inline static const std::string emiterVelocityUniform = "u_emiterVelocity";
};

