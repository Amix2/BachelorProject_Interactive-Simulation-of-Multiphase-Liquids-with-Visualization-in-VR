#version 430 core

#define MAX_FLUID 3072
#define MAX_GLASS 3072
#define MAX_FLUID_TYPES 10
#define MAX_PARTICLE_OBJECTS 10
#define MAX_SPEED 1
#define SORT_ARRAY_SIZE 2*MAX_FLUID
#define MAX_SCENE_X 200
#define MAX_SCENE_Y 200
#define MAX_SCENE_Z 200

#define M_PI 3.1415926535897932384626433832795

#define INSERT_VARIABLES_HERE

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

//////////////////////////////////////////////////
//	STORAGE

struct FluidParticle {
	float x, y, z;
	int type;
};

struct FluidType {
	float mass;
	float stiffness;
	float viscosity;
	float density;
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

layout(std430, binding = 1) buffer positionsBuf
{
	FluidParticle fluidPositions[MAX_FLUID];
};

layout(std430, binding = 10) buffer neighboursBuf
{
	int neighboursBeginInd[27*MAX_FLUID];	// array index of neighbours of set particle
};


layout(std430, binding = 6) buffer detailsBuf
{
	uint numOfParticles;
	uint numOfGlassParticles;
};

layout(std430, binding = 9) buffer sortingHelpBuf
{
	uint sortIndexArray[SORT_ARRAY_SIZE];	// cell number in sorter order
	uint originalIndex[SORT_ARRAY_SIZE];
	FluidParticle	CPY_Positions[MAX_FLUID];
	float	CPY_Velocity[3 * MAX_FLUID];
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
	float fluidDensityPressure[2*MAX_FLUID];
};

layout(std430, binding = 2) buffer glassPartBuf
{
	GlassParticle glassParticles[MAX_GLASS];
};

layout(std140, binding = 3) uniform glassObjectsBuf
{
	GlassObjectDetails glassObjects[MAX_PARTICLE_OBJECTS];
};

//////////////////////////////////////////////////

float Kernel(in float x) {   
	if(x >= 1) return 0.0;
	return 315 * pow(1 - pow(x, 2), 3) / (64 * M_PI);
}

float KernelDerivative(in float x) {  
	if(x >= 1) return 0.0;
	return  -45 * pow(1 - x, 2) / (M_PI);
}

float KernelSecondDerivative(in float x) {  
	if(x >= 1) return 0.0;
	return 45 * (1 - x) / M_PI;         
}


void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
	if(myThreadNumber>=numOfParticles) return;
	const FluidParticle myFluid = fluidPositions[myThreadNumber];
	if(myFluid.type<0) return;
	
	float pDensity = 0;
	float pPressure;
	vec3 pGlassSurfaceVector = vec3(0,0,0);
	float pMinGlassDistance = 99.0f;	// any number higher than Kernel Base

	int neiCount = 0;

	for(int cellIter = int(27*myThreadNumber); cellIter<27 + int(27*myThreadNumber); cellIter++) {

		const uint thisNeiCellIndex = sortIndexArray[neighboursBeginInd[cellIter]]; 

		// for every neighbour in 1 cell starting from first until their cell index change
		for(int neiIter = neighboursBeginInd[cellIter]; thisNeiCellIndex == sortIndexArray[neiIter] && neiIter > -1; neiIter++) {
			const FluidParticle neiPartcie = fluidPositions[neiIter];
			const float dist = distance(vec3(myFluid.x, myFluid.y, myFluid.z), vec3(neiPartcie.x, neiPartcie.y, neiPartcie.z));
			if(dist >= 1) continue;
			neiCount++;
			pDensity += fluidTypeArray[myFluid.type].mass * Kernel(dist);

			if(neiPartcie.type < 0 && dist > 0) {

				const int neiGlassParticleIndex = (neiPartcie.type+1)*-1;	// -1 ==> 0 | -2 ==> 1
				const GlassParticle neiGlassParticle = glassParticles[(-1)*(neiPartcie.type+1)];
				const vec4 neiLocalGlassVector = vec4(neiGlassParticle.vecX, neiGlassParticle.vecY, neiGlassParticle.vecZ, 0.0f);
				const mat4 transformMatrix = glassObjects[neiGlassParticle.glassNumber].matrix;
				const vec4 neiGlobalGlassVector = (transformMatrix * neiLocalGlassVector) / dist;

				pGlassSurfaceVector += neiGlobalGlassVector.xyz;

				if(dist < pMinGlassDistance) pMinGlassDistance = dist; 
			}

		}
	}

	////	FOR EVERY GLASS PARTICLE DO THE SAME

	fluidDensityPressure[2*myThreadNumber] = pDensity;
	fluidDensityPressure[2*myThreadNumber+1] = fluidTypeArray[myFluid.type].stiffness * abs(pDensity - fluidTypeArray[myFluid.type].density);
		
	pGlassSurfaceVector = normalize(pGlassSurfaceVector);

	//fluidSurfaceDistance[myThreadNumber] = pMinGlassDistance;
	fluidSurfaceVector[3*myThreadNumber+0] = pGlassSurfaceVector.x;
	fluidSurfaceVector[3*myThreadNumber+1] = pGlassSurfaceVector.y;
	fluidSurfaceVector[3*myThreadNumber+2] = pGlassSurfaceVector.z;
}


/*
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID; 		== contains the index of the workgroup currently being operated on by a compute shader
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID	== contains the global index of work item currently being operated on by a compute shader
in uint  gl_LocalInvocationIndex;
in uvec3 gl_WorkGroupSize		== layout
*/