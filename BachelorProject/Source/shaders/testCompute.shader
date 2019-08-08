#version 430 core
layout(local_size_x = 510, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f)  uniform image1D csValue;
layout(std430 ,binding = 2) buffer info
{
	float value[];
};
layout(std430, binding = 60) buffer info2
{
	float value2[];
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

void main(void)
{
	int ind = int(globalInvocationIndex());

	float val = 0;
	for (int i = 0; i < 100000; i++) {
		if (i % 2 == 0) {
			val += i;
		} else {
			val -= i;
		}
	}

	value[ind] += value2[ind];
	value[ind] *= -1;
}