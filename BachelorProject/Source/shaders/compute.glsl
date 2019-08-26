#version 430 core

#define SIZE 1
#define CALC 10000

layout(local_size_x = SIZE, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 10) buffer positionsBuf
{
	float positions[];
};

layout(std430, binding = 11) buffer glassPositionsBuf
{
	float glassPositions[];
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

uniform vec3 glassVector[15*1024+1001];

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
	for(uint i=0; i<CALC/SIZE; i++) {
		val ++;
	}
	//numOfParticles = 12;
	//numOfGlassParticles = 13;
	int ind = int(gl_LocalInvocationIndex);
	//for(int i=0; i<1000000; i++) {}

	positions[0] += 0.005;
	if(positions[0] >=1) positions[0] = 0;
	positions[1] -= 0.008;
	if(positions[1] <0) positions[1] = 1;

	positions[1] = glassVector[15*1024+1000].x;
	positions[2] += 0.003;
	if(positions[2] >=1) positions[2] = 0;

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

bool hasNewParticles()
{
	return numOfNewParticles >0;
}

void handleNewParticles() {
	if(gl_LocalInvocationIndex == 0) {
		uint i=0;
		while(i < 3*numOfNewParticles) {
			if(newPartType > 0) {
				positions[3*numOfParticles + i] = newPartPositions[i];
			} else {
				glassPositions[3*numOfGlassParticles + i] = newPartPositions[i];
			}
			i++;
		}
		if(newPartType > 0) {
			numOfParticles += numOfNewParticles;
		} else {
			numOfGlassParticles += numOfNewParticles;
		}
		numOfNewParticles = 0;
	}
}
