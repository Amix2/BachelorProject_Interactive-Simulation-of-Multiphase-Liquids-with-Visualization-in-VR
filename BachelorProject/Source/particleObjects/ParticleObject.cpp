#include "ParticleObject.h"

ParticleObject::ParticleObject() : SelectableObject(){
	m_matrix = glm::mat4(3.0f);
	m_distanceToFurthestParticle = 0;

	setVRActionController<ParticleObjectVRActionController, ParticleObject>(this);
}

std::string ParticleObject::toString() const
{
	std::stringstream ss;
	ss << "Particle Object:\n\tCenter: " << glm::to_string(m_center)<< "\n\t";
	ss << "Curr Mat " << glm::to_string(m_matrix) << "\n\t";
	ss << "Dest Mat " << glm::to_string(m_destinationMatrix) << "\n\t";
	ss << "Stats: dist: " << m_distanceToFurthestParticle << ", rad: " << m_innerRadius << ", hei: " << m_height << ", thi: " << m_thickness;
	return std::string(ss.str());
}

void ParticleObject::init()
{
	//createGlassObject(GraphicShaderStorage::getShader(ShaderNames.GlassObject), GraphicShaderStorage::getShader(ShaderNames.SelectedGlassObject));
	glassObject->init();
}

void ParticleObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	glassObject->load(view, projection);
}

void ParticleObject::createGlassObject(const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram)
{
	glassObject = std::make_shared<GlassObject>(shaderProgram, selectedProgram, (const ParticleObject*) this);
}

glm::mat4 ParticleObject::getModel() const
{
	return m_matrix;
}

void ParticleObject::stepTowardsDestination()
{
	m_matrix = m_destinationMatrix;
}

