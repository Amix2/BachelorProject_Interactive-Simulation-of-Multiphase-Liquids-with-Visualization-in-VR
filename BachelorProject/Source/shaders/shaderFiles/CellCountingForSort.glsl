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

uniform mat4 u_emiterMatrix;
uniform int u_emiterParticlesNumber;
uniform float u_emiterVelocity;

struct FluidParticle {
	float x, y, z;
	int type;
};

struct GlassParticle {
	float localX, localY, localZ;
	float vecX, vecY, vecZ;
	uint glassNumber;
	float _padding;
};

struct GlassObjectDetails {
	mat4 matrix;
};

layout(std430, binding = 1) buffer positionsBuf
{
	FluidParticle fluidPositions[MAX_FLUID];
};

layout(std430, binding = 2) buffer glassPartBuf
{
	GlassParticle glassParticles[MAX_GLASS];
};

layout(std140, binding = 3) uniform glassObjectsBuf
{
	GlassObjectDetails glassObjects[MAX_PARTICLE_OBJECTS];
};

layout(std430, binding = 4) buffer detailsBuf
{
	uint numOfParticles;
	uint numOfGlassParticles;
};

layout(std430, binding = 6) buffer neighboursBuf
{
	int neighboursBeginInd[27*MAX_FLUID];	// array index of neighbours of set particle
};

layout(std430, binding = 7) buffer sortingHelpBuf
{
	uint sortIndexArray[SORT_ARRAY_SIZE];	// cell number in sorter order
	uint originalIndex[SORT_ARRAY_SIZE];
	FluidParticle	CPY_Positions[MAX_FLUID];
	float	CPY_Velocity[3 * MAX_FLUID];
};

layout(std430, binding = 8) buffer simVariablesBuf
{
	float fluidVelocity[3 * MAX_FLUID];
	float fluidAcceleration[3 * MAX_FLUID];
	float fluidSurfaceVector[3 * MAX_FLUID];
	float fluidSurfaceDistance[MAX_FLUID];
	float fluidDensityPressure[2*MAX_FLUID];
};


// section in array to know what particles should be calculated by this thread, ...Last -> 1 after last that should be calculated

//////////////////////////////////////////////////
//	FUNCTIONS

	uint getCellIndex(in float pX, in float pY, in float pZ);

//////////////////////////////////////////////////
//	CODE


void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
	//if(myThreadNumber >= numOfParticles) return;
	if(myThreadNumber >= numOfParticles && numOfParticles + u_emiterParticlesNumber > myThreadNumber) {
		// emiter
		vec3 right = vec3(u_emiterMatrix[0][0], u_emiterMatrix[0][1], u_emiterMatrix[0][2]);
		vec3 up = vec3(u_emiterMatrix[1][0], u_emiterMatrix[1][1], u_emiterMatrix[1][2]);
		vec3 forward = vec3(u_emiterMatrix[2][0], u_emiterMatrix[2][1], u_emiterMatrix[2][2]) * u_emiterVelocity;
		vec3 position = vec3(u_emiterMatrix[3][0], u_emiterMatrix[3][1], u_emiterMatrix[3][2]);
		fluidPositions[myThreadNumber].x = position.x;
		fluidPositions[myThreadNumber].y = position.y;
		fluidPositions[myThreadNumber].z = position.z;
		fluidPositions[myThreadNumber].type = 1;

		fluidVelocity[3*myThreadNumber+0] = forward.x;
		fluidVelocity[3*myThreadNumber+1] = forward.y;
		fluidVelocity[3*myThreadNumber+2] = forward.z;

		atomicAdd(numOfParticles, 1);
	}
	FluidParticle myParticle = fluidPositions[myThreadNumber];
	const int myType = myParticle.type;


	if(myType < 0) {
		// its a glass particle
		//const int myGlassParticleIndex = int((-1)*(myType+1));	// -1 ==> 0 | -2 ==> 1
		const GlassParticle myGlassParticle = glassParticles[(-1)*(myType+1)];
		const vec4 localPos = vec4(myGlassParticle.localX, myGlassParticle.localY, myGlassParticle.localZ, 1.0f);
		const mat4 transformMatrix = glassObjects[myGlassParticle.glassNumber].matrix;
		const vec4 globalPos =  transformMatrix * localPos;
		const vec3 lastPosition = vec3(myParticle.x, myParticle.y, myParticle.z);
		const vec3 velosity = globalPos.xyz - lastPosition;

		myParticle.x = globalPos.x;
		myParticle.y = globalPos.y;
		myParticle.z = globalPos.z;
		fluidPositions[myThreadNumber].x = globalPos.x;
		fluidPositions[myThreadNumber].y = globalPos.y;
		fluidPositions[myThreadNumber].z = globalPos.z;

		fluidVelocity[3*myThreadNumber+0] = velosity.x;
		fluidVelocity[3*myThreadNumber+1] = velosity.y;
		fluidVelocity[3*myThreadNumber+2] = velosity.z;
	}

	const uint cellIndex = getCellIndex(myParticle.x, myParticle.y, myParticle.z);

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