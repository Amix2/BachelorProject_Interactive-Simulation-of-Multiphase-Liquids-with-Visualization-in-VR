#version 430 core
layout(local_size_x = 900, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 10) buffer positionsBuf
{
	int atom;
	float positions[];
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

vec3 getVec3FromData(in float data[9], in int num) {
	return vec3(data[3 * num], data[3 * num + 1], data[3 * num + 2]);
}

void putVec3FromData(inout float data[9], in int num, in vec3 values) {
	positions[3 * num] = values.x;
	positions[3 * num + 1] = values.y;
	positions[3 * num + 2] = values.z;
}

void main(void)
{
	int ind = int(globalInvocationIndex());

	//vec3 pos = getVec3FromData(positions, ind);
	//pos.x *= temp[ind];
	//positions[3 * ind] = pos.x;
	//barrier();
	for(int i=0; i<1000*ind; i++) {}
	if(ind%2 == 0)
		atomicAdd(atom, 1);
		for(int i=0; i<1000*ind; i++) {}
barrier();
	positions[ind] = atom;
}
