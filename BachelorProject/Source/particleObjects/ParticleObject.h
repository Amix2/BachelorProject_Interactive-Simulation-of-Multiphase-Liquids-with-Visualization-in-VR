#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include <Logger.h>
#include <Configuration.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <particleObjects/ParticleGeometry.h>
#include <dataStructures/ParticleData.h>
#include <selectableObject/SelectableObject.h>
#include <memory>
#include <materialObjects/MaterialObjectOwner.h>
#include <materialObjects/GlassObject.h>
#include <utilities/GraphicShaderStorage.h>

class GlassObject;
struct ParticleObjectDetais {
	int fluidType;	// -1 -> glass | >0 -> index in fluid type array | 0 -> reserved
	union {
		struct {
			// for liquid box
			float lowX, lowY, lowZ;
			float highX, highY, highZ;
		};
		struct {
			// for mugs / glass
			float cX, cY, cZ; // center of inner mug
			float innerRadius;
			float thickness;
			float height;
		};
	};
};

// Represents single object like mug etc
class ParticleObject
	: public SelectableObject, public MaterialObjectOwner
{
protected:
	bool m_selected = false;
	std::shared_ptr<GlassObject> glassObject;

public:
	glm::mat4 m_matrix;

	glm::vec3 m_center;
	float m_innerRadius;
	float m_height;
	float m_thickness;
	float m_distanceToFurthestParticle = 0;

	glm::mat4 m_destinationMatrix;

	ParticleObject();

	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
	void createGlassObject(const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram);

	glm::mat4 getModel() const;

	virtual void stepTowardsDestination();

	virtual std::string toString() const;
};
