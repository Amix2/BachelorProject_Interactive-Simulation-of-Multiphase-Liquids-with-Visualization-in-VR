#version 430 core

#define NUM_THREADS 1
#define MAX_FLUID 8192
#define MAX_GLASS 3072
#define MAX_PARTICLE_OBJECTS 10
#define MAX_SCENE_X 200
#define MAX_SCENE_Y 200
#define MAX_SCENE_Z 200
#define SORT_ARRAY_SIZE 2*MAX_FLUID
#define EMITER_FLUID_PARTICLE_BUILD_GAP 10.0f
#define MAX_EMITERS 5

#define INSERT_VARIABLES_HERE


layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;


//////////////////////////////////////////////////
//	STORAGE


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

struct Emiter {
	mat4 matrix;
	float velocity;
	int emitThisTurn;
	int fluidType;
	float rotationAngle;
};

layout(std430, binding = 1) buffer positionsBuf
{
	FluidParticle fluidPositions[];
};

layout(std430, binding = 2) buffer glassPartBuf
{
	GlassParticle glassParticles[];
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
	int neighboursBeginInd[];	// array index of neighbours of set particle
};

layout(std430, binding = 7) buffer sortingHelpBuf
{
	uint sortIndexArray[SORT_ARRAY_SIZE];	// cell number in sorter order
	uint originalIndex[SORT_ARRAY_SIZE];
};

layout(std430, binding = 8) buffer simVariablesBuf
{
	float fluidVelocity[3 * MAX_FLUID];
	float fluidAcceleration[3 * MAX_FLUID];
	float fluidSurfaceVector[3 * MAX_FLUID];
	float fluidSurfaceDistance[MAX_FLUID];
	float fluidDensityPressure[2*MAX_FLUID];
};

layout(std140, binding = 10) uniform emitersBuf
{
	Emiter emiterArray[MAX_EMITERS];
};


// section in array to know what particles should be calculated by this thread, ...Last -> 1 after last that should be calculated

//////////////////////////////////////////////////
//	FUNCTIONS

	uint getCellIndex(in float pX, in float pY, in float pZ);

//////////////////////////////////////////////////
//	CODE
mat4 rotationMatrix(vec3 axis, float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;

	//if(myThreadNumber >= numOfParticles) return;
	if(myThreadNumber >= numOfParticles) {
		// emiter
		int indOffset = int(myThreadNumber) - int(numOfParticles);
		int emiterID = 0;
		Emiter emiter = emiterArray[0];
		while(emiterID < MAX_EMITERS && indOffset >= 0) {
			if(indOffset - emiter.emitThisTurn*emiter.emitThisTurn >= 0) {
				indOffset -= emiter.emitThisTurn*emiter.emitThisTurn;
				emiterID ++;
				emiter = emiterArray[emiterID];
		
			} else {
				break;
			}
		}
		
		if(emiterID != MAX_EMITERS){

			const vec3 forward = vec3(emiter.matrix[2][0], emiter.matrix[2][1], emiter.matrix[2][2]);
			const mat4 rotationMatrix = rotationMatrix(forward, emiter.rotationAngle);

			const vec4 right = rotationMatrix * vec4(emiter.matrix[0][0], emiter.matrix[0][1], emiter.matrix[0][2], 0.0);
			const vec4 up = rotationMatrix * vec4(emiter.matrix[1][0], emiter.matrix[1][1], emiter.matrix[1][2], 0.0);
			const vec3 position = vec3(emiter.matrix[3][0], emiter.matrix[3][1], emiter.matrix[3][2]);



			const int numOfParticlesInRow = emiter.emitThisTurn;



			const vec3 myEmitPosition = position 
				+ (indOffset % numOfParticlesInRow - (numOfParticlesInRow-1) * 0.5) * EMITER_FLUID_PARTICLE_BUILD_GAP * right.xyz 
				+ (indOffset / numOfParticlesInRow - (numOfParticlesInRow-1) * 0.5) * EMITER_FLUID_PARTICLE_BUILD_GAP * up.xyz;

			if(length(position - myEmitPosition) <= emiter.emitThisTurn * EMITER_FLUID_PARTICLE_BUILD_GAP * 0.5) {
				fluidPositions[myThreadNumber].x = myEmitPosition.x;
				fluidPositions[myThreadNumber].y = myEmitPosition.y;
				fluidPositions[myThreadNumber].z = myEmitPosition.z;
				fluidPositions[myThreadNumber].type = emiter.fluidType;

				fluidVelocity[3*myThreadNumber+0] = forward.x * emiter.velocity;
				fluidVelocity[3*myThreadNumber+1] = forward.y * emiter.velocity;
				fluidVelocity[3*myThreadNumber+2] = forward.z * emiter.velocity;
			}

		}

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