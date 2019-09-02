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
	addFluidType(99, 88, 77, 66);
	addFluidType(199, 188, 177, 166);
	GpuResources::createUBO(BufferDatails.fluidTypesName, Configuration.MAX_FLUID_TYPES * sizeof(FluidType), m_fluidTypes, BufferDatails.fluidTypesBinding);
}
