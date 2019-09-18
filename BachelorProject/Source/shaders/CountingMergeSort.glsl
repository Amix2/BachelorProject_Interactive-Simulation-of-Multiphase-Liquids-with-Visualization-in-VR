#version 430 core
/*
INFO:
	Work Group Size == (1, 1, 1)
	Starting Number Of Work Groups == max power of 2 less than number of particles
ASSUMPTIONS:
	let id = this thread ID
	let numOfTh = total number of threads 
	let last_interval = SORT_ARRAY_SIZE / 2*numOfTh = interval on sort array from last dispatch

	this thread has to merge arrays 2*id*last_interval AND 2*(id+1)*last_interval
	into 2*id*last_interval on another array (A or B, look uniform bool use_A)
	Array sortIndex 
*/

#define NUM_THREADS 1
#define MAX_FLUID 8192
#define SORT_ARRAY_SIZE 2*MAX_FLUID

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


//////////////////////////////////////////////////
//	STORAGE

uniform bool use_A;

layout(std430, binding = 9) buffer sortingHelpBuf
{
	uint sortIndex_A[SORT_ARRAY_SIZE];
	uint count_A[SORT_ARRAY_SIZE];
	uint sortIndex_B[SORT_ARRAY_SIZE];
	uint count_B[SORT_ARRAY_SIZE];
	float	CPY_Positions[3 * MAX_FLUID];
	float	CPY_Velocity[3 * MAX_FLUID];
	int		CPY_FluidTypes[MAX_FLUID];
};



void main(void)
{

}

/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/