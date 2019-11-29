#include "Emiter.h"

Emiter::Emiter(int initNumberOfParticles, float initVelocity, int initFluidType, ShaderProgram pyramidShader) 
	: m_numOfParticles(initNumberOfParticles), m_fluidType(initFluidType), MaterialObject(pyramidShader)
	, SelectableObject()
{

	setVRActionController<EmiterVRActionController, Emiter>(this);

	m_Velocity = min(initVelocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);

	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));
	setRender(true);
	m_pyramid = std::make_unique<PyramidPointerMaterialObject>(shaderProgram, glm::vec4{ 0.55f, 0.0f, 0.55f, 1.0 }, this);


}

Emiter::Emiter(int initNumberOfParticles, float initVelocity, int initFluidType, ShaderProgram pyramidShader, glm::vec3 postion)
	: Emiter(initNumberOfParticles, initVelocity, initFluidType, pyramidShader)
{
	m_Matrix = glm::mat4(1);
	Utils::setPosition(&m_Matrix, postion);
}

Emiter::Emiter(int initNumberOfParticles, float initVelocity, int initFluidType, ShaderProgram pyramidShader, glm::mat4 matrix)
	: Emiter(initNumberOfParticles, initVelocity, initFluidType, pyramidShader)
{
	m_Matrix = matrix;
}


int Emiter::fillGPUdata(GPUEmiter* data, int turnNumber)
{
	if (!m_isActive) {
		data->emitThisTurn = 0;
		m_emitThisTurn = 0;
		return 0;
	}


	if (turnNumber % m_emitFrequency == 0 && ParticleData::m_NumOfParticles + 512 + m_numOfParticles < Configuration.MAX_PARTICLES) {
		m_rotationAngle += 0.1;
		data->matrix = m_Matrix * glm::rotate(glm::mat4{ 1 }, m_rotationAngle, glm::vec3(0,0,1));
		data->velocity = m_Velocity;
		data->fluidType = m_fluidType;
		m_emitThisTurn = m_numOfParticles;
		data->emitThisTurn = m_emitThisTurn;
		return m_emitThisTurn * m_emitThisTurn;
	}
	else {
		m_emitThisTurn = 0;
		data->emitThisTurn = 0;
		return 0;
	}
}

glm::mat4 Emiter::getModel() const
{
	return m_Model;
}

std::map<Params, MultiTypeValue> Emiter::getAdditionalParameters() const
{
	MultiTypeValue color;
	if (isSelected()) {
		if (m_isActive) {
			color.vec4Value = COLOR_SELECTED_ACTIVE;
		}
		else {
			color.vec4Value = COLOR_SELECTED_NOT_ACTIVE;
		}
	}
	else {
		if (m_isActive) {
			color.vec4Value = COLOR_NOT_SELECTED_ACTIVE;
		}
		else {
			color.vec4Value = COLOR_NOT_SELECTED_NOT_ACTIVE;
		}
	}
	return std::map<Params, MultiTypeValue>({ {Params::COLOR, color} });
}

void Emiter::init()
{
	m_pyramid->init();
}

void Emiter::load(const glm::mat4& view, const glm::mat4& projection) const
{
	if (this->getRender()) {

		m_Model = m_Matrix * glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 2, glm::vec3(1, 0, 0));
		m_Model = m_Model * glm::scale(glm::mat4{ 1.0f }, { m_numOfParticles,m_numOfParticles * 0.5f,m_numOfParticles });
		Utils::setPosition(&m_Model, Utils::getPosition(m_Model) - Utils::getForward(m_Matrix) * (((float)m_numOfParticles) * 0.5f));

		m_pyramid->load(view, projection);

		m_Model = m_Model * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 3, glm::vec3(0,1, 0));

		m_pyramid->load(view, projection);
		//LOG_F(WARNING, "Emiter pos:\n%s\n%s", glm::to_string(m_Matrix).c_str(), glm::to_string(m_Model).c_str());
	}
}

std::string Emiter::toString()
{
	std::stringstream ss;
	ss << "Emiter :: active: \t" << (m_isActive ? "true" : "false") << "\n\tParticles: " << m_numOfParticles << ", this turn: " << m_emitThisTurn
		<< "\n\tVel: " << m_Velocity
		<< "\n\tFrequency: " << m_emitFrequency
		<< "\n\tMat ["<< (m_updateMatrix ? "update" : "no-update") <<"]: " << glm::to_string(m_Matrix);
	return std::string(ss.str());
}

void Emiter::grab()
{
	m_selected = true;
	m_isActive = true;
}

void Emiter::release()
{
	m_selected = false;
	m_isActive = false;
}

glm::mat4* Emiter::getMatrix()
{
	return &(m_Matrix);
}

void Emiter::setMatrix(const glm::mat4& matrix)
{
	m_Matrix = matrix;
}

void Emiter::updateMatrix(const glm::mat4& matrix)
{
	if (m_updateMatrix) {
		m_Matrix = matrix;
	}
}



int Emiter::changeSize(int rowsNumber)
{
	m_numOfParticles += rowsNumber;
	if (m_numOfParticles <= 0) m_numOfParticles = 1;
	return m_numOfParticles;
}

int Emiter::increaseSize(int addRows)
{
	m_numOfParticles += addRows;
	return m_numOfParticles;
}

int Emiter::decreaseSize(int revemeRows)
{
	m_numOfParticles -= revemeRows;
	if (m_numOfParticles <= 0) m_numOfParticles = 1;
	return m_numOfParticles;
}

float Emiter::increaseVelocity(int stepsUp)
{
	for(int i=stepsUp; i>0; i--)
		m_Velocity *= 1.1f;
	m_Velocity = min(m_Velocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);
	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));
	return m_Velocity;
}

float Emiter::decreaseVelocity(int stepsDown)
{
	for (int i = stepsDown; i > 0; i--) {
		m_Velocity *= 0.9f;
		if (m_Velocity < 1.0f) m_Velocity = 1.0f;
	}
	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));
	return m_Velocity;
}

float Emiter::chengeVeloccity(float defVelocity)
{
	m_Velocity += defVelocity;
	m_Velocity = min(m_Velocity, Configuration.MAX_PARTICLE_SPEED / Configuration.DELTA_TIME);
	m_emitFrequency = int(ceil(Configuration.EMITER_FLUID_PARTICLE_BUILD_GAP / (m_Velocity * Configuration.DELTA_TIME)));
	return m_Velocity;
}
