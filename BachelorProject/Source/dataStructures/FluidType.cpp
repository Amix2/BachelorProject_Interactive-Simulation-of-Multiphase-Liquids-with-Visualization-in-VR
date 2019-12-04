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
	addFluidType(65, 8000, 4500, 100, { 1.0f, 0.0f, 0.0f, 1.0f });

	addFluidType(20, 5000, 300, 10, { 0.0f, 1.0f, 0.0f, 1.0f });
	addFluidType(250, 5000, 3000, 100, { 0.5f, 0.5f, 0.5f, 1.0f });


	//addFluidType(150, 8000, 4500, 100, { 0.0f, 0.0f, 1.0f, 1.0f });
	addFluidType(150, 2000, 1000, 20, { 0.0f, 0.0f, 1.0f, 1.0f });

	//addFluidType(199, 188, 177, 166);
}

// fluidTypeArray[myFluid.type].stiffness * abs(outDensity - fluidTypeArray[myFluid.type].density);