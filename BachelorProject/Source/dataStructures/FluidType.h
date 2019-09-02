#pragma once
#include <Configuration.h>
#include <dataStructures/GpuResources.h>

class FluidType
{
	float m_mass;
	float m_stiffness;
	float m_viscosity;
	float m_density;
	FluidType(float mass, float stiffness, float viscosity, float density) : m_mass(mass), m_stiffness(stiffness), m_viscosity(viscosity), m_density(density) {}
	FluidType() : m_mass(0), m_stiffness(0), m_viscosity(0), m_density(0) {}
public:
	static FluidType m_fluidTypes[Configuration.MAX_FLUID_TYPES];
	inline static int m_numOfFluidTypes = 0;

	static int addFluidType(float mass, float stiffness, float viscosity, float density);
	static void init();
};
