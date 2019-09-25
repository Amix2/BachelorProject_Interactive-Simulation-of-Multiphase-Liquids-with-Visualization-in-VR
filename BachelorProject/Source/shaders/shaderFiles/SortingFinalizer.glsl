#version 430 core

#define NUM_THREADS 1
#define MAX_FLUID 8192
#define MAX_SCENE_X 200
#define MAX_SCENE_Y 200
#define MAX_SCENE_Z 200
#define SORT_ARRAY_SIZE 2*MAX_FLUID

#define INSERT_VARIABLES_HERE


layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;


//////////////////////////////////////////////////
//	STORAGE

layout(std430, binding = 9) buffer sortingHelpBuf
{
	uint sortIndexArray[SORT_ARRAY_SIZE];
	uint originalIndex[SORT_ARRAY_SIZE];
	float	CPY_Positions[3 * MAX_FLUID];
	float	CPY_Velocity[3 * MAX_FLUID];
	int		CPY_FluidTypes[MAX_FLUID];
};

layout(std430, binding = 1) buffer positionsBuf
{
	float fluidPositions[3*MAX_FLUID];
};

layout(std430, binding = 2) buffer partFluidTypeBuf
{
	int particleFluidType[MAX_FLUID];
};


layout(std430, binding = 6) buffer detailsBuf
{
	uint numOfParticles;
	uint numOfGlassParticles;
};

layout(std430, binding = 8) buffer simVariablesBuf
{
	float fluidVelocity[3 * MAX_FLUID];
	float fluidAcceleration[3 * MAX_FLUID];
	float fluidSurfaceVector[3 * MAX_FLUID];
	float fluidSurfaceDistance[MAX_FLUID];
	float fluidDensity[MAX_FLUID];
	float fluidPressure[MAX_FLUID];
};

void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x
			+ gl_LocalInvocationIndex;
	const uint myTakeFromId = originalIndex[myThreadNumber];
	const uint myInsertToId = myThreadNumber;
	if(myThreadNumber>=numOfParticles || myTakeFromId == myInsertToId) return;

	fluidPositions[3*myInsertToId+0] = CPY_Positions[3*myTakeFromId+0];
	fluidPositions[3*myInsertToId+1] = CPY_Positions[3*myTakeFromId+1];
	fluidPositions[3*myInsertToId+2] = CPY_Positions[3*myTakeFromId+2];

	fluidVelocity[3*myInsertToId+0] = CPY_Velocity[3*myTakeFromId+0];
	fluidVelocity[3*myInsertToId+1] = CPY_Velocity[3*myTakeFromId+1];
	fluidVelocity[3*myInsertToId+2] = CPY_Velocity[3*myTakeFromId+2];

	particleFluidType[myInsertToId] = CPY_FluidTypes[myTakeFromId];
	
}
/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/