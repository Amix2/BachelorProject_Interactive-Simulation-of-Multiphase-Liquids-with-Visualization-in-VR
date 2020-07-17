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
#define MAX_SCENE_X 200
#define MAX_SCENE_Y 200
#define MAX_SCENE_Z 200
#define SORT_ARRAY_SIZE 2*MAX_FLUID

#define INSERT_VARIABLES_HERE


layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;


//////////////////////////////////////////////////

uniform int u_stage;
uniform int u_turnInStage;

layout(std430, binding = 7) buffer sortingHelpBuf
{
	uint sortIndexArray[SORT_ARRAY_SIZE];
	uint originalIndex[];
};

//////////////////////////////////////////////////

void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
	const uint TwoPowDifStageTurn = uint(pow(2, u_stage - u_turnInStage));
	const uint myFirst = myThreadNumber + uint(floor(float(myThreadNumber) / TwoPowDifStageTurn) * TwoPowDifStageTurn);
	const uint myFirstValue = sortIndexArray[myFirst];
	const uint mySecond = myFirst + TwoPowDifStageTurn;
	const uint mySecondValue = sortIndexArray[mySecond];

	//const uint myGroupNumber = uint(myThreadNumber / pow(2, u_stage - 1));

	//const bool dir_up = myGroupNumber%2 == 0;	// true -> first > last || false -> first < last

	// if(dir_up) {
	if(uint(myThreadNumber / pow(2, u_stage - 1))%2 == 0) {	// UP
		if(myFirstValue < mySecondValue) {
			sortIndexArray[myFirst] = mySecondValue;
			sortIndexArray[mySecond] = myFirstValue;
			const uint temp = originalIndex[myFirst];
			originalIndex[myFirst] = originalIndex[mySecond];
			originalIndex[mySecond] = temp;
		} 
	} else {	// DOWN
		if(myFirstValue > mySecondValue) {
			sortIndexArray[myFirst] = mySecondValue;
			sortIndexArray[mySecond] = myFirstValue;
			const uint temp = originalIndex[myFirst];
			originalIndex[myFirst] = originalIndex[mySecond];
			originalIndex[mySecond] = temp;
		}
	}

}

/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/