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
#define DELTA_TIME 0.0005
#define GRAVITY_Y -3000;

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
	vec4 color;
};

struct GlassObjectDetails {
	mat4 matrix;
};

layout(std430, binding = 1) buffer positionsBuf
{
	FluidParticle fluidPositions[];
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
	float fluidDensityPressure[2*MAX_FLUID];
	float glassForceMultiplier[MAX_FLUID];
};

shared float shFluidSurfaceDistance[270];
shared float shFluidAcceleration[3 * 270];

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

#define GLASS_KERNEL_MULTIPLIER 1
float GlassKernelDerivative(in float x) {  
	if(x >= 1) return 0.0;
	return  -45 * pow(1 - GLASS_KERNEL_MULTIPLIER*(x-(1-1/GLASS_KERNEL_MULTIPLIER)), 2) / (M_PI);
}

float GlassKernelSecondDerivative(in float x) {  
	if(x >= 1) return 0.0;
	return 45 * (1 - GLASS_KERNEL_MULTIPLIER*(x-(1-1/GLASS_KERNEL_MULTIPLIER))) / M_PI;         
}


void main(void)
{
	const uint myThreadNumber = gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
	const uint myParticleIndex = myThreadNumber / 27;
	const uint myLocalGroupNumber = gl_LocalInvocationIndex / 27;
	counters[myLocalGroupNumber] = 0;

	const uint mySharedIndex = gl_LocalInvocationIndex;

	if(myParticleIndex>=numOfParticles) {
		return;	
	}
	const FluidParticle myFluid = fluidPositions[myParticleIndex];
	if(myFluid.type<0) {
		return;
	}
	
	vec3 pPressureVec = vec3(0,0,0);
	vec3 pViscosityVec = vec3(0,0,0);
	vec3 pAcceleration = vec3(0,0,0);

	float pMinGlassDistance = 99.0f;	// any number higher than Kernel Base

	const FluidType myType = fluidTypeArray[myFluid.type];
	const float pDensity =	fluidDensityPressure[2*myParticleIndex+0];
	const float pPressure = fluidDensityPressure[2*myParticleIndex+1];
	const vec3 pVelocity = vec3(fluidVelocity[3*myParticleIndex+0], fluidVelocity[3*myParticleIndex+1], fluidVelocity[3*myParticleIndex+2]); 
	const float pGlassMultiplier = glassForceMultiplier[myParticleIndex];

	int neiIter = neighboursBeginInd[myThreadNumber];
	const vec3 myFluidPosition = vec3(myFluid.x, myFluid.y, myFluid.z);

	//for(int cellIter = int(27*myThreadNumber); cellIter<27 + int(27*myThreadNumber); cellIter++) {
	if(neiIter > -1) {
		const uint thisNeiCellIndex = sortIndexArray[neiIter];

		// for every neighbour in 1 cell starting from first until their cell index change
		//for(int neiParticleIndex = neighboursBeginInd[cellIter]; thisNeiCellIndex == sortIndexArray[neiParticleIndex] && neiParticleIndex > -1; neiParticleIndex++) {
		while(thisNeiCellIndex == sortIndexArray[neiIter]) {

			const FluidParticle neiPartcie = fluidPositions[neiIter];
			vec3 neiVelocity = vec3(fluidVelocity[3*neiIter+0], fluidVelocity[3*neiIter+1], fluidVelocity[3*neiIter+2]); 

			const float dist = distance(myFluidPosition, vec3(neiPartcie.x, neiPartcie.y, neiPartcie.z));
			if(dist >= 1 || dist == 0) {
				neiIter++;
				continue;
			}

			const vec3 direction = normalize(myFluidPosition - vec3(neiPartcie.x, neiPartcie.y, neiPartcie.z));
			if(neiPartcie.type < 0) {	// glass
				const vec3 pGlassSurfaceVector = vec3(fluidSurfaceVector[3*myParticleIndex+0], fluidSurfaceVector[3*myParticleIndex+1], fluidSurfaceVector[3*myParticleIndex+2]);
	
				const float cosAngle = dot(direction, pGlassSurfaceVector);
				//if(dist < pMinGlassDistance) pMinGlassDistance = dist;
				const float thisSurfaceDist = cosAngle * dist;
				if(thisSurfaceDist < pMinGlassDistance) pMinGlassDistance = thisSurfaceDist;

				const float tPressSc = myType.mass * (2*pPressure/pow(pDensity, 2) ) * KernelDerivative(dist);
				pPressureVec += direction * tPressSc * pGlassMultiplier;


			} else {	// if fluid;
				const float neiDensity = fluidDensityPressure[2*neiIter+0];
				const float neiPressure = fluidDensityPressure[2*neiIter+1];

				const float tPressSc = myType.mass * (pPressure/pow(pDensity, 2) + neiPressure/pow(neiDensity, 2)) * KernelDerivative(dist);
				pPressureVec += direction * tPressSc;

				const float tVescSc = myType.mass * myType.viscosity / pDensity / neiDensity * KernelSecondDerivative(dist);
				pViscosityVec += (neiVelocity - pVelocity) * tVescSc;
			}
			neiIter++;

			//fluidSurfaceVector[3*myThreadNumber+0] = dist;
		}
	}

	pAcceleration = pViscosityVec - pPressureVec;
	shFluidAcceleration[3*mySharedIndex+0] = pAcceleration.x;
	shFluidAcceleration[3*mySharedIndex+1] = pAcceleration.y;
	shFluidAcceleration[3*mySharedIndex+2] = pAcceleration.z;

	shFluidSurfaceDistance[mySharedIndex] = pMinGlassDistance;

	//memoryBarrierShared();
	if(atomicAdd(counters[myLocalGroupNumber], 1) == 26) {

		float outSurfDist, outAccX, outAccY, outAccZ;
		outSurfDist = 99.0f;
		outAccX = outAccY = outAccZ = 0;
		for(int i=0; i<27; i++) {
			const float tempMinDistance = shFluidSurfaceDistance[27*myLocalGroupNumber+i];
			if(tempMinDistance < outSurfDist) 
				outSurfDist = tempMinDistance;

			outAccX += shFluidAcceleration[3*(27*myLocalGroupNumber+i) + 0];
			outAccY += shFluidAcceleration[3*(27*myLocalGroupNumber+i) + 1];
			outAccZ += shFluidAcceleration[3*(27*myLocalGroupNumber+i) + 2];
		}

		fluidAcceleration[3*myParticleIndex+0] = outAccX;
		fluidAcceleration[3*myParticleIndex+1] = outAccY;
		fluidAcceleration[3*myParticleIndex+2] = outAccZ;

		fluidSurfaceDistance[myParticleIndex] = outSurfDist;
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