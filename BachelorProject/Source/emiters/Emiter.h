#pragma once
#include <glm/glm.hpp>
#include <Configuration.h>
#include <sstream>
#include <emiters/EmiterProvider.h>
#include <glm/gtx/string_cast.hpp>
#include <dataStructures/ParticleData.h>
#include <selectableObject/SelectableObject.h>
#include <materialObjects/MaterialObjectOwner.h>

struct GPUEmiter {
	glm::mat4 matrix;
	float velocity;
	int emitThisTurn;
	int fluidType;
	float _padding;
};

class Emiter
	: public SelectableObject

{
private:
	int m_emitFrequency	= 0;
	float m_Velocity	= 0;
	int m_emitThisTurn	= 0;
	int m_numOfParticles	= 0;
	int m_fluidType = 0;
	bool m_isActive		= false;
	float m_rotationAngle	= 0;
	bool m_updateMatrix = true;
	glm::mat4 m_Matrix	= glm::mat4();
	bool m_selected = false;
public:
	Emiter(int initNumberOfParticles, float initVelocity, int initFluidType);
	Emiter() {}

	int fillGPUdata(GPUEmiter* data, int turnNumber);


	bool isSelected() const override { return m_selected; }
	void grab() override;
	void release() override;
	glm::mat4* getMatrix() override;
	void setMatrix(const glm::mat4& matrix) override;

	void updateMatrix(const glm::mat4 &matrix);

	int changeSize(int rowsNumber);
	int increaseSize(int addRows);
	int decreaseSize(int revemeRows);
	float increaseVelocity(int stepsUp=1);
	float decreaseVelocity(int stepsDown=1);
	float chengeVeloccity(float defVelocity);
	void setActive(bool active) { m_isActive = active; }
	void toggleActive() { m_isActive = !m_isActive; }
	void setMatrixUpdate(bool update) { m_updateMatrix = update; }
	void togleMatrixUpdate() { m_updateMatrix = !m_updateMatrix; }



	std::string toString();
};

