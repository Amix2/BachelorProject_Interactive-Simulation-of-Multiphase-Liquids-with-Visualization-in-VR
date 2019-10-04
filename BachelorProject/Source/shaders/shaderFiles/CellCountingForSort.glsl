#version 430 core

#define NUM_THREADS 1
#define MAX_FLUID 8192
#define MAX_GLASS 3072
#define MAX_PARTICLE_OBJECTS 10
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
	int type;
};

struct GlassParticle {
	float localX, localY, localZ;
	float vecX, vecY, vecZ;
	uint glassNumber;
	float __padding;
};

struct GlassObjectDetails {
	mat4 matrix;
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

layout(std140, binding = 2) uniform glassPartBuf
{
	GlassParticle glassParticles[MAX_GLASS];
};

layout(std140, binding = 3) uniform glassObjectsBuf
{
	GlassObjectDetails glassObjects[MAX_PARTICLE_OBJECTS];
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
	FluidParticle myParticle = fluidPositions[myThreadNumber];
	const int myType = myParticle.type;
	if(myType < 0) {
		// its a glass particle
		const int myGlassParticleIndex = (myType+1)*-1;	// -1 ==> 0 | -2 ==> 1
		const GlassParticle myGlassParticle = glassParticles[myGlassParticleIndex];
		const vec4 localPos = vec4(myGlassParticle.localX, myGlassParticle.localY, myGlassParticle.localZ, 1.0f);
		const mat4 transformMatrix = glassObjects[myGlassParticle.glassNumber].matrix;
		const vec4 globalPos = transformMatrix * localPos;

		myParticle.x = globalPos.x;
		myParticle.y = globalPos.y;
		myParticle.z = globalPos.z;
		fluidPositions[myThreadNumber].x = globalPos.x;
		fluidPositions[myThreadNumber].y = globalPos.y;
		fluidPositions[myThreadNumber].z = globalPos.z;
	}

	sortIndexArray[myThreadNumber] = getCellIndex(myParticle.x, myParticle.y, myParticle.z);
	originalIndex[myThreadNumber] = myThreadNumber;

}

uint getCellIndex(in float pX, in float pY, in float pZ)  {
	if(pX<=0 || pX>=MAX_SCENE_X || pY<=0 || pY>=MAX_SCENE_Y || pZ<=0 || pX>=MAX_SCENE_Z) {
		return 0;
	}
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