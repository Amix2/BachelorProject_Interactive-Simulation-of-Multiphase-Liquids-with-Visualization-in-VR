#pragma once

#include <emiters/EmiterProvider.h>
#include <Configuration.h>
#include <Logger.h>
#include <glm/gtx/string_cast.hpp>
#include <emiters/Emiter.h>
#include <memory>
#include <Logger.h>
#include <selectableObject/SelectableObjectManager.h>
#include <materialObjects/PyramidPointerMaterialObject.h>
#include <scene/Scene.h>
#include <mutex>

class EmiterManager
{
private:
	inline static int m_initState = 0;
	inline static int m_TARGET_INIT_STATE = 3;

	inline static std::vector<Emiter> m_emitersVector;
	inline static ShaderProgram m_pyramidShader;

	inline static std::mutex m_mutex;

public:

	static Emiter* createEmiter(int initNumberOfParticles, float initVelocity, int initFluidType, bool selectable=true);
	static Emiter* createEmiter(int initNumberOfParticles, float initVelocity, int initFluidType, glm::vec3 position, bool selectable = true);
	static Emiter* createEmiter(int initNumberOfParticles, float initVelocity, int initFluidType, glm::mat4 matrix, bool selectable = true);

	static void printEmiters();

	static int updateAllEmiters(int turnNumber);

	static void init(ShaderProgram pyramid);


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

