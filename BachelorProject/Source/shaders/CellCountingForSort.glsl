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

struct FluidParticle {
	float x, y, z;
	uint type;
};

layout(std430, binding = 9) buffer sortingHelpBuf
{
	uint sortIndexArray[SORT_ARRAY_SIZE];
	uint originalIndex[SORT_ARRAY_SIZE];
	FluidParticle	CPY_Positions[MAX_FLUID];
	float	CPY_Velocity[3 * MAX_FLUID];
};

layout(std430, binding = 1) buffer positionsBuf
{
	FluidParticle fluidPositions[MAX_FLUID];
};


layout(std430, binding = 6) buffer detailsBuf
{
	uint numOfParticles;
	uint numOfGlassParticles;
};

layout(std430, binding = 3) buffer glassPositionsBuf
{
	float glassPositions[3*MAX_FLUID];
};


// section in array to know what particles should be calculated by this thread, ...Last -> 1 after last that should be calculated

//////////////////////////////////////////////////
//	FUNCTIONS

	void findMyArraySections(out uint myFluidFirst, out uint myFluidLast, out uint mySortFirst);

	uint getCellIndex(in float pX, in float pY, in float pZ);

//////////////////////////////////////////////////
//	CODE


void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
	//if(particleFluidType[myThreadNumber] != 0) {
		sortIndexArray[myThreadNumber] = getCellIndex(fluidPositions[myThreadNumber].x, fluidPositions[myThreadNumber].y, fluidPositions[myThreadNumber].z);
		originalIndex[myThreadNumber] = myThreadNumber;
	//}

//	if(myFluidFirst == myFluidLast) {
//		// This thread takes care only for 1 particle
//		const uint cell = getCellIndex(	fluidPositions[3*myFluidFirst + 0]
//										, fluidPositions[3*myFluidFirst + 1]
//										, fluidPositions[3*myFluidFirst + 2]);
//		sortIndex[mySortFirst] = cell;
//		count[mySortFirst] = 1;
//	} else {
//	// This thread takes care for 2 particles
//		const uint cellFirst = getCellIndex(fluidPositions[3*myFluidFirst + 0]
//											, fluidPositions[3*myFluidFirst + 1]
//											, fluidPositions[3*myFluidFirst + 2]);
//		const uint cellLast	 = getCellIndex(fluidPositions[3*myFluidLast + 0]
//											, fluidPositions[3*myFluidLast + 1]
//											, fluidPositions[3*myFluidLast + 2]);
//		if(cellFirst < cellLast) {
//			sortIndex[mySortFirst] = cellFirst;
//			count[mySortFirst] = 1;
//			sortIndex[mySortFirst] = cellLast;
//			count[mySortFirst] = 1;
//		} else if(cellFirst == cellLast) {
//			sortIndex[mySortFirst] = cellFirst;
//			count[mySortFirst] = 2;
//		} else {	// cellFirst > cellLast
//			sortIndex[mySortFirst] = cellLast;
//			count[mySortFirst] = 1;
//			sortIndex[mySortFirst] = cellFirst;
//			count[mySortFirst] = 1;
//		}
//	}

}


void findMyArraySections(out uint myFluidFirst, out uint myFluidLast, out uint mySortFirst) {
	const uint numOfThreads = gl_WorkGroupSize.x
		* gl_NumWorkGroups.x;
		//	= num of Work Groups * size of Work Group

	const uint myThreadNumber = max(gl_WorkGroupID.x - 1, 0) 
		* gl_WorkGroupSize.x
		+ gl_LocalInvocationIndex;
		//	= num of Work Groups before * size of Work Group + my Id in this Work Group

	// calculate first and last FLUID index for this thread (later refered as "my")
	const float fluidInterval = numOfParticles / numOfThreads;
	myFluidFirst = uint(round(myThreadNumber * fluidInterval));
	if(myThreadNumber < numOfThreads - 1) 
		myFluidLast = uint(round((myThreadNumber + 1) * fluidInterval));
	else
		myFluidLast = numOfParticles;
	
	mySortFirst = myThreadNumber * uint(SORT_ARRAY_SIZE / numOfThreads);
}

uint getCellIndex(in float pX, in float pY, in float pZ)  {
	return uint(pX) + MAX_SCENE_X * uint(pZ) + MAX_SCENE_X * MAX_SCENE_Z * uint(pY);
}

/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/