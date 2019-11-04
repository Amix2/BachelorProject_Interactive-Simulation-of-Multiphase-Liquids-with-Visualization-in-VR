#include "FluidType.h"

FluidType FluidType::m_fluidTypes[Configuration.MAX_FLUID_TYPES];

int FluidType::addFluidType(float mass, float stiffness, float viscosity, float density)
{
	m_fluidTypes[m_numOfFluidTypes] = FluidType(mass, stiffness, viscosity, density);
	m_numOfFluidTypes++;
	return m_numOfFluidTypes;
}

void FluidType::init()
{
	// define all avaliable fluid types
	//	mass, stiffness, viscosity, density
	//addFluidType(65, 5000, 4500, 90);
	addFluidType(65, 8000, 3000, 100);
	//addFluidType(199, 188, 177, 166);
}
