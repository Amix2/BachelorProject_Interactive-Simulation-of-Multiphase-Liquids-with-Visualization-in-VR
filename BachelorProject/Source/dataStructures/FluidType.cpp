#include "FluidType.h"

FluidType FluidType::m_fluidTypes[Configuration.MAX_FLUID_TYPES];

int FluidType::addFluidType(float mass, float stiffness, float viscosity, float density, glm::vec4 color)
{
	m_fluidTypes[m_numOfFluidTypes] = FluidType(mass, stiffness, viscosity, density, color);
	m_numOfFluidTypes++;
	return m_numOfFluidTypes;
}

void FluidType::init()
{
	// define all avaliable fluid types
	//	mass, stiffness, viscosity, density
	//addFluidType(65, 5000, 4500, 90);

	addFluidType(65, 8000, 3000, 200, { 0.7f, 0.2f, 0.6f, 1.0f });
	addFluidType(650, 8000, 3000, 200, { 0.7f, 0.1f, 0.1f, 1.0f });
	//addFluidType(199, 188, 177, 166);
}
