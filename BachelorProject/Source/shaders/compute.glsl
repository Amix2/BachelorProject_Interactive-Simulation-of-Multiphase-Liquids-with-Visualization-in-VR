#version 430 core

#define LOCAL_SIZE_X 6//&

layout(local_size_x = LOCAL_SIZE_X, local_size_y = 1, local_size_z = 1) in;

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
	int ind = int(gl_LocalInvocationIndex);
	if(hasNewParticles()) {
		handleNewParticles();
	}
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
	return newPartPositions[0] != -1;
}

void handleNewParticles() {
	if(gl_LocalInvocationIndex == 0) {
		uint i=0;
		while(newPartPositions[i] != -1) {
			positions[3*numOfParticles + i] = newPartPositions[i];
			newPartPositions[i] = -1;
			i++;
		}
		numOfParticles += numOfNewParticles;
		numOfNewParticles = 0;
	}
}
