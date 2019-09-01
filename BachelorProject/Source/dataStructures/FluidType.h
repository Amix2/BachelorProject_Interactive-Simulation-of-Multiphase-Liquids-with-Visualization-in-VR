#pragma once
#include <Configuration.h>
#include <particleObjects/ParticleObject.h>

	class FluidType
	{
		const float m_mass;
		const float m_stiffness;
		const float m_viscosity;
		const float m_density;
	public:
		FluidType(float mass, float stiffness, float viscosity, float density) : m_mass(mass), m_stiffness(stiffness), m_viscosity(viscosity), m_density(density) {}
	};

class FluidTypes
{
public:
	inline static FluidType m_fluidTypes[Configuration.MAX_FLUID_TYPES];
	inline static int m_numOfFluidTypes = 0;

	static void addFluidType(float mass, float stiffness, float viscosity, float density);
	static void addFluidType(const FluidType& fluid);

	static void createBasic();
};
