#version 430 core


#define NUM_THREADS 10 
#define MAX_FLUID 3072
#define MAX_GLASS 3072
#define MAX_FLUID_TYPES 10
#define MAX_PARTICLE_OBJECTS 10
#define MAX_SPEED 1

#define INSERT_VARIABLES_HERE

layout(local_size_x = NUM_THREADS, local_size_y = 1, local_size_z = 1) in;

//////////////////////////////////////////////////
//	STRUCT

uint myGlassFirst, myGlassLast;
uint myParticleFirst, myParticleLast;

struct ParticleObject {

	vec3 currentPosition;
	vec3 targetPosition;

	vec3 currentVector;
	vec3 targetVector;

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

//////////////////////////////////////////////////
//	STORAGE

layout(std430, binding = 10) buffer positionsBuf
{
	float positions[3*MAX_FLUID];
};

layout(std430, binding = 16) buffer partFluidTypeBuf
{
	int particleFluidType[MAX_FLUID];
};

layout(std430, binding = 11) buffer glassPositionsBuf
{
	float glassPositions[3*MAX_GLASS];
};

layout(std140, binding = 14) uniform glassVectorsBuf
{
	float glassVectors[3*MAX_GLASS];
};

layout(std430, binding = 12) buffer objectsBuf
{
	ParticleObject objects[];
};

layout(std430, binding = 13) buffer detailsBuf
{
	uint numOfParticles;
	uint numOfGlassParticles;
};

layout(std140, binding = 15) uniform fluidTypesBuf
{
	FluidType fluidTypeArray[MAX_FLUID_TYPES];
};

layout(std430, binding = 20) buffer simVariablesBuf
{
	float xd;
};

//////////////////////////////////////////////////
//	FUNCTIONS

void moveObjects();	// move and rotate particle objects

//////////////////////////////////////////////////
//	CODE

/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/

uint globalInvocationIndex() {
	const uvec3 size = gl_WorkGroupSize + uvec3(1, 1, 1);
	return gl_GlobalInvocationID.z * size.x * size.y + gl_GlobalInvocationID.y * size.x + gl_GlobalInvocationID.x;
}

void main(void)
{

	// calculate first and last GLASS index for this thread (later refered as "my")
	const uint glassInterval = uint(round(numOfGlassParticles / NUM_THREADS));
	myGlassFirst = gl_LocalInvocationIndex * glassInterval;
	if(gl_LocalInvocationIndex < NUM_THREADS - 1) 
		myGlassLast = (gl_LocalInvocationIndex + 1)* glassInterval;
	else
		myGlassLast = numOfGlassParticles;

	// calculate first and last PARTICLE index for this thread (later refered as "my")
	const uint particleInterval = uint(round(numOfParticles / NUM_THREADS));
	myParticleFirst = gl_LocalInvocationIndex * particleInterval;
	if(gl_LocalInvocationIndex < NUM_THREADS - 1) 
		myParticleLast = (gl_LocalInvocationIndex + 1)* particleInterval;
	else
		myParticleLast = numOfParticles;


	for(int i=0; i<numOfParticles; i++) {
		positions[3*i] = float(particleFluidType[i]);
	}

	//positions[0] = 99;
	//if(hasNewParticles()) {
	//	handleNewParticles();
	//}
	//vec3 pos = getVec3FromData(positions, ind);
	//pos.x *= temp[ind];
	//positions[3 * ind] = pos.x;
	//barrier();
	//for(int i=0; i<1000*ind; i++) {}
	//barrier();
	//positions[ind] = 2;//newPartPositions[ind];
}

void moveObjects() {

}
