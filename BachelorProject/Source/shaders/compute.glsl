#version 430 core


#define NUM_THREADS 3
#define MAX_FLUID 3072
#define MAX_GLASS 3072
#define MAX_FLUID_TYPES 10
#define MAX_PARTICLE_OBJECTS 10
#define MAX_SPEED 1

#define INSERT_VARIABLES_HERE

layout(local_size_x = NUM_THREADS, local_size_y = 1, local_size_z = 1) in;

//////////////////////////////////////////////////
//	STRUCT

struct ParticleObject {

	vec4 currentPosition;
	vec4 targetPosition;

	vec4 currentVector;
	vec4 targetVector;

	int indBegin;
	int indEnd;

	float _padding[2];
};

struct FluidType {
	float mass;
	float tiffness;
	float viscosity;
	float density;
};

struct FluidParticle {
	float x, y, z;
	uint type;
};

//////////////////////////////////////////////////
//	STORAGE

layout(std430, binding = 1) buffer positionsBuf
{
	FluidParticle fluidPositions[MAX_FLUID];
};

layout(std430, binding = 2) buffer partFluidTypeBuf
{
	int particleFluidType[MAX_FLUID];
};

layout(std430, binding = 3) buffer glassPositionsBuf
{
	float glassPositions[3*MAX_GLASS];
};

layout(std140, binding = 4) buffer glassVectorsBuf
{
	float glassVectors[3*MAX_GLASS];
};

layout(std430, binding = 5) buffer objectsBuf
{
	ParticleObject objects[];
};

layout(std430, binding = 6) buffer detailsBuf
{
	uint numOfParticles;
	uint numOfGlassParticles;
};

layout(std140, binding = 7) uniform fluidTypesBuf
{
	FluidType fluidTypeArray[MAX_FLUID_TYPES];
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

//////////////////////////////////////////////////
//	FUNCTIONS
	/*	setup	uint myGlassFirst, myGlassLast;
				uint myFluidFirst, myFluidLast;
		to know what particles should be calculated by this thread
	*/
	void findMyArraySections();

//////////////////////////////////////////////////
//	CODE


void main(void)
{

	// SPH
	for(int i=0; i<numOfParticles; i++) {
		fluidPositions[i].y -= 0.05;	// move particles
	}
}


//void findMyArraySections2() {
//	const uint numOfThreads = gl_WorkGroupSize.x * gl_WorkGroupSize.y * gl_WorkGroupSize.z 
//		* gl_NumWorkGroups.x * gl_NumWorkGroups.y * gl_NumWorkGroups.z;
//		//	= num of Work Groups * size of Work Group
//
//	const uint myThreadNumber = max(gl_WorkGroupID.x * gl_WorkGroupID.y * gl_WorkGroupID.z - 1, 0) 
//		* (gl_WorkGroupSize.x * gl_WorkGroupSize.y * gl_WorkGroupSize.z)
//		+ gl_LocalInvocationIndex;
//		//	= num of Work Groups before * size of Work Group + my Id in this Work Group
//
//	// calculate first and last GLASS index for this thread (later refered as "my")
//	const uint glassInterval = uint(round(numOfGlassParticles / numOfThreads));
//	myGlassFirst = myThreadNumber * glassInterval;
//	if(myThreadNumber < numOfThreads - 1) 
//		myGlassLast = (myThreadNumber + 1) * glassInterval;
//	else
//		myGlassLast = numOfGlassParticles;
//
//	// calculate first and last FLUID index for this thread (later refered as "my")
//	const uint fluidInterval = uint(round(numOfParticles / numOfThreads));
//	myFluidFirst = myThreadNumber * fluidInterval;
//	if(myThreadNumber < numOfThreads - 1) 
//		myFluidLast = (myThreadNumber + 1) * fluidInterval;
//	else
//		myFluidLast = numOfParticles;
//}

/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/