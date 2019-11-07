#pragma once
#include <shaders/ComputeShader.h>
#include <simulationObjects/EmiterMatrixProvider.h>
#include <Configuration.h>
#include <Logger.h>
#include <glm/gtx/string_cast.hpp>
#include <dataStructures/ParticleData.h>
#include <inputDispatcher/InputDispatcher.h>
#include <simulationObjects/EmiterControler.h>
#include <memory>

class Emiter
{
private:
	inline static int m_initState = 0;
	inline static int m_TARGET_INIT_STATE= 3;
public:
	inline static ComputeShader* m_emiterComputeShader = nullptr;
	inline static EmiterMatrixProvider* m_matrixProvider;
	inline static InputDispatcher* m_inputDispatcher;
	inline static std::unique_ptr<EmiterControler> m_controler = std::make_unique<EmiterControler>();

	inline static glm::mat4 m_emiterMatrix;
	inline static int m_emitFrequancy;
	inline static float m_emiterVelocity = -1.0f;
	inline static int m_emiterdThisTurn = 0;

	static void setEmiter(EmiterMatrixProvider* matrixProvider, int numOfParticles, float velocity);
	static void setInputDispatcher(InputDispatcher* inputDispatcher);

	static void updateTurn(int turnNumber);

	static void setEmiterComputeShader(ComputeShader* shader);

	inline static const std::string emiterMatrixUniform = "u_emiterMatrix";
	inline static const std::string emiterParticlesNumberUnifom = "u_emiterParticlesNumber";
	inline static const std::string emiterVelocityUniform = "u_emiterVelocity";
};

