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

layout(local_size_x = 270, local_size_y = 1, local_size_z = 1) in;

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

layout(std140, binding = 5) uniform fluidTypesBuf
{
	FluidType fluidTypeArray[MAX_FLUID_TYPES];
};

layout(std430, binding = 6) buffer neighboursBuf
{
	int neighboursBeginInd[];	// array index of neighbours of set particle
};

layout(std430, binding = 7) buffer sortingHelpBuf
{
	uint sortIndexArray[];	// cell number in sorter order
};

layout(std430, binding = 8) buffer simVariablesBuf
{
	float fluidVelocity[3 * MAX_FLUID];
	float fluidAcceleration[3 * MAX_FLUID];
	float fluidSurfaceVector[3 * MAX_FLUID];
	float fluidSurfaceDistance[MAX_FLUID];
	float fluidDensityPressure[];
};


shared float shDensity[270];
shared float shSurfaceVector[3 * 270];

shared uint counters[10];

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
	const uint myParticleIndex = myThreadNumber / 27;
	
	const uint myLocalGroupNumber = gl_LocalInvocationIndex / 27;
	counters[myLocalGroupNumber] = 0;


	if(myParticleIndex>=numOfParticles) {
		return;	
	}

	const uint mySharedIndex = gl_LocalInvocationIndex;

	const FluidParticle myFluid = fluidPositions[myParticleIndex];
	if(myFluid.type<0) {
		return;	// exit if my particle in a glass
	}
	
	float pDensity = 0;
	

	vec3 pGlassSurfaceVector = vec3(0,0,0);

	int neiIter = neighboursBeginInd[myThreadNumber];
	const vec3 myFluidPosition = vec3(myFluid.x, myFluid.y, myFluid.z);

	if(neiIter > -1) {
		const uint thisNeiCellIndex = sortIndexArray[neiIter]; 
	
		// for every neighbour in 1 cell starting from first until their cell index change
		while(thisNeiCellIndex == sortIndexArray[neiIter]) {
			const FluidParticle neiPartcie = fluidPositions[neiIter];
			neiIter++;
			const float dist = distance(myFluidPosition, vec3(neiPartcie.x, neiPartcie.y, neiPartcie.z));
			if(dist >= 1) continue;

			pDensity += fluidTypeArray[myFluid.type].mass * Kernel(dist);

			if(neiPartcie.type < 0 && dist > 0) {	// if GLASS

				const GlassParticle neiGlassParticle = glassParticles[ -(neiPartcie.type+1) ];	// -1 ==> 0 | -2 ==> 1
				const vec4 neiLocalGlassVector = vec4(neiGlassParticle.vecX, neiGlassParticle.vecY, neiGlassParticle.vecZ, 0.0f);
				const mat4 transformMatrix = glassObjects[neiGlassParticle.glassNumber].matrix;
				const vec4 neiGlobalGlassVector = (transformMatrix * neiLocalGlassVector) / dist;

				pGlassSurfaceVector += neiGlobalGlassVector.xyz;
			}

		}
	}


	shDensity[mySharedIndex] = pDensity;
		
	pGlassSurfaceVector = normalize(pGlassSurfaceVector);

	shSurfaceVector[3*mySharedIndex+0] = pGlassSurfaceVector.x;
	shSurfaceVector[3*mySharedIndex+1] = pGlassSurfaceVector.y;
	shSurfaceVector[3*mySharedIndex+2] = pGlassSurfaceVector.z;


	if(atomicAdd(counters[myLocalGroupNumber], 1) == 26) {
		float outDensity, outVecX, outVecY, outVecZ;
		outDensity = outVecX = outVecY = outVecZ = 0;
		for(int i=0; i<27; i++) {
			outDensity += shDensity[27*myLocalGroupNumber+i];
			outVecX += shSurfaceVector[3*(27*myLocalGroupNumber+i) + 0];
			outVecY += shSurfaceVector[3*(27*myLocalGroupNumber+i) + 1];
			outVecZ += shSurfaceVector[3*(27*myLocalGroupNumber+i) + 2];
		}

		fluidDensityPressure[2*myParticleIndex] = outDensity;
		fluidDensityPressure[2*myParticleIndex+1] = fluidTypeArray[myFluid.type].stiffness * abs(outDensity - fluidTypeArray[myFluid.type].density);

		const float vecLen = sqrt(outVecX * outVecX + outVecY * outVecY + outVecZ * outVecZ);

		fluidSurfaceVector[3*myParticleIndex+0] = outVecX / vecLen;
		fluidSurfaceVector[3*myParticleIndex+1] = outVecY / vecLen;
		fluidSurfaceVector[3*myParticleIndex+2] = outVecZ / vecLen;
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