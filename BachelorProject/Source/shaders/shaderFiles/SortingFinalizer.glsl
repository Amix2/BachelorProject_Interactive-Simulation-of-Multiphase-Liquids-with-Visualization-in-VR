#version 430 core

#define NUM_THREADS 1
#define MAX_FLUID 8192
#define MAX_SCENE_X 200
#define MAX_SCENE_Y 200
#define MAX_SCENE_Z 200
#define MIN_SCENE_X 200
#define MIN_SCENE_Y 200
#define MIN_SCENE_Z 200
#define SORT_ARRAY_SIZE 2*MAX_FLUID

#define INSERT_VARIABLES_HERE


layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;


//////////////////////////////////////////////////
//	STORAGE

struct FluidParticle {
	float x, y, z;
	int type;
};

layout(std430, binding = 1) buffer positionsBuf
{
	FluidParticle fluidPositions[];
};

layout(std430, binding = 4) buffer detailsBuf
{
	uint numOfParticles;
	uint numOfGlassParticles;
};

layout(std430, binding = 7) buffer sortingHelpBuf
{
	uint sortIndexArray[SORT_ARRAY_SIZE];	// cell number in sorter order
	uint originalIndex[SORT_ARRAY_SIZE];
	FluidParticle	CPY_Positions[MAX_FLUID];
	float	CPY_Velocity[];
};

layout(std430, binding = 8) buffer simVariablesBuf
{
	float fluidVelocity[];
};

layout(std430, binding = 9) buffer lookUpTableBuf
{
	int indexMap[];	// array index of neighbours of set particle
};

void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
	const uint myTakeFromId = originalIndex[myThreadNumber];

	if(myThreadNumber>=numOfParticles) return;

	if(sortIndexArray[myThreadNumber] == 0) {
		fluidPositions[myThreadNumber].x = MIN_SCENE_X;
		fluidPositions[myThreadNumber].y = MIN_SCENE_Y;
		fluidPositions[myThreadNumber].z = MIN_SCENE_Z;
		fluidPositions[myThreadNumber].type = 0;

		fluidVelocity[3*myThreadNumber+0] = 0;
		fluidVelocity[3*myThreadNumber+1] = 0;
		fluidVelocity[3*myThreadNumber+2] = 0;

		atomicAdd(numOfParticles, -1);
	} else {

		fluidPositions[myThreadNumber] = CPY_Positions[myTakeFromId];

		fluidVelocity[3*myThreadNumber+0] = CPY_Velocity[3*myTakeFromId+0];
		fluidVelocity[3*myThreadNumber+1] = CPY_Velocity[3*myTakeFromId+1];
		fluidVelocity[3*myThreadNumber+2] = CPY_Velocity[3*myTakeFromId+2];

		if(myThreadNumber == 0 || sortIndexArray[myThreadNumber] != sortIndexArray[myThreadNumber-1]) {
			indexMap[sortIndexArray[myThreadNumber]] = int(myThreadNumber);
		}
	}

	
}
/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/