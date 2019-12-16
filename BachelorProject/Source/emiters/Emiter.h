#pragma once
#include <glm/glm.hpp>
#include <Configuration.h>
#include <sstream>
#include <glm/gtx/string_cast.hpp>
#include <dataStructures/ParticleData.h>
#include <selectableObject/SelectableObject.h>
#include <materialObjects/MaterialObject.h>
#include <materialObjects/MaterialObjectOwner.h>
#include <materialObjects/PyramidPointerMaterialObject.h>
#include <scene/Scene.h>

class PyramidPointerMaterialObject;

struct GPUEmiter {
	glm::mat4 matrix;
	float velocity;
	int emitThisTurn;
	int fluidType;
	float _padding;
};

class Emiter
	: public SelectableObject
	, public MaterialObject
	, public MaterialObjectOwner
{
private:
	inline static glm::vec4 COLOR_SELECTED_ACTIVE =			glm::vec4(0.8f,		0.8f,	0.8f,	1.f );
	inline static glm::vec4 COLOR_SELECTED_NOT_ACTIVE =		glm::vec4(0.8f,		0.35f,	0.8f,	1.f );
	inline static glm::vec4 COLOR_NOT_SELECTED_ACTIVE =		glm::vec4(0.8f,		0.8f,	0.35f,	1.f );
	inline static glm::vec4 COLOR_NOT_SELECTED_NOT_ACTIVE = glm::vec4(0.8f,		0.35f,	0.35f,	1.f );

	int m_emitFrequency	= 0;
	float m_Velocity	= 0;
	int m_emitThisTurn	= 0;
	int m_numOfParticles	= 0;
	int m_fluidType = 0;
	bool m_isActive		= false;
	float m_rotationAngle	= 0;
	glm::mat4 m_Matrix	= glm::mat4();
	bool m_selected = false;

	mutable glm::mat4 m_Model;

	std::unique_ptr<PyramidPointerMaterialObject> m_pyramid;

public:
	Emiter(int initNumberOfParticles, float initVelocity, int initFluidType, ShaderProgram pyramidShader);
	Emiter(int initNumberOfParticles, float initVelocity, int initFluidType, ShaderProgram pyramidShader, glm::mat4 matrix);
	Emiter(int initNumberOfParticles, float initVelocity, int initFluidType, ShaderProgram pyramidShader, glm::vec3 postion);
	Emiter() {}

	int fillGPUdata(GPUEmiter* data, int turnNumber);

	bool isSelected() const override { return m_selected; }
	void grab() override;
	void release() override;
	glm::mat4* getMatrix() override;

	void updateMatrix(const glm::mat4 &matrix);

	int changeSize(int rowsNumber);
	int increaseSize(int addRows);
	int decreaseSize(int revemeRows);
	float increaseVelocity(int stepsUp=1);
	float decreaseVelocity(int stepsDown=1);
	float chengeVelocity(float defVelocity);
	void setActive(bool active) { m_isActive = active; }
	void setFluidType(int type) { m_fluidType = type; }
	void toggleActive() { m_isActive = !m_isActive; }
	void setMatrixUpdate(bool update) { m_updateMatrix = update; }
	void togleMatrixUpdate() { m_updateMatrix = !m_updateMatrix; }

	glm::mat4 getModel() const override;

	std::map<Params, MultiTypeValue> getAdditionalParameters() const override;

	void init();
	void load(const glm::mat4& view, const glm::mat4& projection) const;

	bool m_updateMatrix = true;

	std::string toString();
};

