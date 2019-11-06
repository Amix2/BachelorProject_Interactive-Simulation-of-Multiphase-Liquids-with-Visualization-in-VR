#pragma once
#include <Configuration.h>
#include <dataStructures/GpuResources.h>

// stores fluid types data, for now they must be define in init() function 
class FluidType
{
	// private data to make objects for static array
	FluidType(float mass, float stiffness, float viscosity, float density, glm::vec4 color) 
		: m_mass(mass)
		, m_stiffness(stiffness)
		, m_viscosity(viscosity)
		, m_density(density)
		, m_color(color){}
	FluidType() : m_mass(0), m_stiffness(0), m_viscosity(0), m_density(0) {}
public:
	float m_mass;
	float m_stiffness;
	float m_viscosity;
	float m_density;
	glm::vec4 m_color;
	// array to hold all avaliable fluid types
	static FluidType m_fluidTypes[Configuration.MAX_FLUID_TYPES];
	inline static int m_numOfFluidTypes = 1;

	static int addFluidType(float mass, float stiffness, float viscosity, float density, glm::vec4 color);
	static void init();
};
