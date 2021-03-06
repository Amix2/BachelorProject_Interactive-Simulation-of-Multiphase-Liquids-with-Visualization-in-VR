#version 430 core

#define MAX_FLUID 3072
#define MAX_GLASS 3072
#define MAX_FLUID_TYPES 10
#define MAX_PARTICLE_OBJECTS 10
#define MAX_SPEED 1
#define SORT_ARRAY_SIZE 2*MAX_FLUID
#define MAX_SCENE_X 200
#define MAX_SCENE_Y 200
#define MAX_SCENE_Z 200
#define MIN_SCENE_X 200
#define MIN_SCENE_Y 200
#define MIN_SCENE_Z 200

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

layout(std430, binding = 6) buffer neighboursBuf
{
	int neighboursBeginInd[];
};

layout(std430, binding = 7) buffer sortingHelpBuf
{
	uint sortIndexArray[];
};

layout(std430, binding = 9) buffer lookUpTableBuf
{
	int indexMap[];	// array index of neighbours of set particle
};

//////////////////////////////////////////////////

uint getCellIndex(in float pX, in float pY, in float pZ)  {
	if(pX<=MIN_SCENE_X || pX>=MAX_SCENE_X || pY<=MIN_SCENE_Y || pY>=MAX_SCENE_Y || pZ<=MIN_SCENE_Z || pX>=MAX_SCENE_Z) {
		return 0;
	}
	return uint(pX-MIN_SCENE_X) + (MAX_SCENE_X-MIN_SCENE_X) * uint(pZ-MIN_SCENE_Z) + (MAX_SCENE_X-MIN_SCENE_X) * (MAX_SCENE_Z-MIN_SCENE_Z) * uint(pY-MIN_SCENE_Y);
}


void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
	const uint myParticleIndex = myThreadNumber / 27;
	const FluidParticle myParticle = fluidPositions[myParticleIndex];

	if(myParticle.type<0) return;

	const vec3 offsetArray[] = vec3[] (
		vec3(-1, -1, -1),
		vec3(-1, -1, 0),
		vec3(-1, -1, 1),
		vec3(-1, 0, -1),
		vec3(-1, 0, 0),
		vec3(-1, 0, 1),
		vec3(-1, 1, -1),
		vec3(-1, 1, 0),
		vec3(-1, 1, 1),
		vec3(0, -1, -1),
		vec3(0, -1, 0),
		vec3(0, -1, 1),
		vec3(0, 0, -1),
		vec3(0, 0, 0),
		vec3(0, 0, 1),
		vec3(0, 1, -1),
		vec3(0, 1, 0),
		vec3(0, 1, 1),
		vec3(1, -1, -1),
		vec3(1, -1, 0),
		vec3(1, -1, 1),
		vec3(1, 0, -1),
		vec3(1, 0, 0),
		vec3(1, 0, 1),
		vec3(1, 1, -1),
		vec3(1, 1, 0),
		vec3(1, 1, 1)
	);


	const uint myOffsetIndex = myThreadNumber % 27;
	const uint myOutputIndex = myThreadNumber;


	//const int myPartX = int(myParticle.x-MIN_SCENE_X);
	//const int myPartY = int(myParticle.y-MIN_SCENE_Y);
	//const int myPartZ = int(myParticle.z-MIN_SCENE_Z);

	const vec3 myOffset = offsetArray[myOffsetIndex];

	const float neighbourX = myParticle.x + (myOffset.x);
	const float neighbourY = myParticle.y + (myOffset.y);
	const float neighbourZ = myParticle.z + (myOffset.z);

	const uint neighbourCellIndex = getCellIndex(neighbourX, neighbourY, neighbourZ);


	int neiIndex = indexMap[neighbourCellIndex];
	if(neighbourCellIndex == 0 || sortIndexArray[neiIndex] != neighbourCellIndex) {
		neiIndex = -1;

	}

	neighboursBeginInd[myOutputIndex] = neiIndex;

}


/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/



////////////////////////////////////////
// BINARY SEARCH

//	if(neighbourCellIndex == 0) {
//		neighboursBeginInd[myOutputIndex] = -1;
//		return;
//	}
//
//	int lower = int(numOfParticles);
//	int higher = 0;
//	int mid = (lower + higher)/2;
//
//	int limit = 0;
//	while(sortIndexArray[mid] != neighbourCellIndex && higher+1 < lower && limit < 10000) 
//	{
//	limit++;
//		if(sortIndexArray[mid] > neighbourCellIndex) {	//	H _ _ _ M _ x _ L
//			higher = mid;
//		}
//		else {	// sortIndexArray[mid] < neighbourCellIndex 	H _ x _ M _ _ _ L
//			lower = mid;
//		}
//		mid = (lower + higher)/2;
//	}
//
//	if(sortIndexArray[mid] != neighbourCellIndex ) {
//		mid = -1;
//	} else {
//		while(sortIndexArray[mid] == neighbourCellIndex) {
//			mid --;
//		}
//		mid++;
//	}
//	neighboursBeginInd[myOutputIndex] = mid;