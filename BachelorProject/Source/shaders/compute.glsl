#version 430 core


#define NUM_THREADS 1 
#define MAX_FLUID 3072
#define MAX_GLASS 3072

#define INSERT_VARIABLES_HERE



layout(local_size_x = NUM_THREADS, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 10) buffer positionsBuf
{
	float positions[3*MAX_FLUID];
};

layout(std140, binding = 11) uniform glassPositionsBuf
{
	float glassPositions[3*MAX_GLASS];
};

layout(std430, binding = 12) buffer newParticlesBuf
{
	int newPartType;
	int numOfNewParticles;
	float newPartPositions[];
};

layout(std430, binding = 13) buffer detailsBuf
{
	uint numOfParticles;
	uint numOfGlassParticles;
};

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

void handleNewParticles();
bool hasNewParticles();

void main(void)
{
	int val = 0;
	//numOfParticles = 12;
	//numOfGlassParticles = 13;
	int ind = int(gl_LocalInvocationIndex);
	//for(int i=0; i<1000000; i++) {}

	positions[0] += 0.005;
	if(positions[0] >=1) positions[0] = 0;
	positions[1] -= 0.008;
	if(positions[1] <0) positions[1] = 1;

	positions[2] += 0.003;
	if(positions[2] >=1) positions[2] = 0;
	positions[3] = glassPositions[0];

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
