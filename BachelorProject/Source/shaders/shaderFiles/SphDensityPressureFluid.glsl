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
	vec4 color;
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
	float fluidDensityPressure[2*MAX_FLUID];
	float glassForceMultiplier[MAX_FLUID];
	float glassMaxVelocity[MAX_FLUID];
	int numOfFluidNeighbours[MAX_FLUID];
};


shared float shDensity[270];
shared float shSurfaceVector[3 * 270];
shared float shGlassMultiplier[270];
shared float shMaxGlassVelocity[270];
shared int shNumOfNeighbours[270];
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
	float distanceToFluid = 0;
	float distanceToGlass = 1;
	float maxGlassVelocity = 0;
	int numOFNeighbours = 0;

	int neiIter = neighboursBeginInd[myThreadNumber];
	const vec3 myFluidPosition = vec3(myFluid.x, myFluid.y, myFluid.z);
	int qq = 0;
	if(neiIter > -1) {
		const uint thisNeiCellIndex = sortIndexArray[neiIter]; 
	
		// for every neighbour in 1 cell starting from first until their cell index change
		while(thisNeiCellIndex == sortIndexArray[neiIter]) {
			const FluidParticle neiPartcie = fluidPositions[neiIter];
			const float dist = distance(myFluidPosition, vec3(neiPartcie.x, neiPartcie.y, neiPartcie.z));
			qq ++;
			if(dist >= 1) {
				neiIter++;
				continue;
			}

			pDensity += fluidTypeArray[myFluid.type].mass * Kernel(dist);
			if(neiPartcie.type < 0 && dist > 0) {	// if GLASS

				const GlassParticle neiGlassParticle = glassParticles[ -(neiPartcie.type+1) ];	// -1 ==> 0 | -2 ==> 1
				const vec4 neiLocalGlassVector = vec4(neiGlassParticle.vecX, neiGlassParticle.vecY, neiGlassParticle.vecZ, 0.0f);
				const mat4 transformMatrix = glassObjects[neiGlassParticle.glassNumber].matrix;
				const vec4 neiGlobalGlassVector = (transformMatrix * neiLocalGlassVector) ;

				pGlassSurfaceVector += neiGlobalGlassVector.xyz;

				distanceToGlass += 1+Kernel(dist);

				const float glassVelocityLen = length(vec3(fluidVelocity[3*neiIter+0], fluidVelocity[3*neiIter+1], fluidVelocity[3*neiIter+2]));
				if(glassVelocityLen > maxGlassVelocity) maxGlassVelocity = glassVelocityLen;
			} else if(neiPartcie.type > 0) {	// if fluid not glass
				numOFNeighbours++;
				distanceToFluid += 1+Kernel(dist);
			}
			neiIter++;
			if(qq > 800) break;
		}
	}

	//atomicAdd(fluidPositions[myParticleIndex].type, 1000);

	shDensity[mySharedIndex] = pDensity;
		
	shSurfaceVector[3*mySharedIndex+0] = pGlassSurfaceVector.x;
	shSurfaceVector[3*mySharedIndex+1] = pGlassSurfaceVector.y;
	shSurfaceVector[3*mySharedIndex+2] = pGlassSurfaceVector.z;
	shGlassMultiplier[mySharedIndex+0] = distanceToFluid;

	//shDistanceToParts[2*mySharedIndex+1] = distanceToGlass;

	shMaxGlassVelocity[mySharedIndex] = maxGlassVelocity;
	shNumOfNeighbours[mySharedIndex] = numOFNeighbours;
	//memoryBarrierShared();
	//barrier();
	//if(mySharedIndex%27 == 0) {
	if(atomicAdd(counters[myLocalGroupNumber], 1) == 26) {
		float outDensity =0, outVecX=0, outVecY=0, outVecZ=0, sumGlassMultiplier=0, sumDistToGlass=0, outMaxGlassVelocity=0;
		int outNumOfNeighbours = 0;
		//outDensity = outVecX = outVecY = outVecZ = 0;
		for(int i=0; i<27; i++) {
			outDensity += shDensity[27*myLocalGroupNumber+i];
			outVecX += shSurfaceVector[3*(27*myLocalGroupNumber+i) + 0];
			outVecY += shSurfaceVector[3*(27*myLocalGroupNumber+i) + 1];
			outVecZ += shSurfaceVector[3*(27*myLocalGroupNumber+i) + 2];
			sumGlassMultiplier += shGlassMultiplier[(27*myLocalGroupNumber+i)];
			if(shMaxGlassVelocity[27*myLocalGroupNumber+i] > outMaxGlassVelocity) outMaxGlassVelocity = shMaxGlassVelocity[27*myLocalGroupNumber+i];
			outNumOfNeighbours += shNumOfNeighbours[27*myLocalGroupNumber+i];
		}

		fluidDensityPressure[2*myParticleIndex] = outDensity;
		fluidDensityPressure[2*myParticleIndex+1] = fluidTypeArray[myFluid.type].stiffness * abs(outDensity - fluidTypeArray[myFluid.type].density);

		const float vecLen = sqrt(outVecX * outVecX + outVecY * outVecY + outVecZ * outVecZ);


		fluidSurfaceVector[3*myParticleIndex+0] = outVecX / vecLen;
		fluidSurfaceVector[3*myParticleIndex+1] = outVecY / vecLen;
		fluidSurfaceVector[3*myParticleIndex+2] = outVecZ / vecLen;
		glassForceMultiplier[myParticleIndex] = sumGlassMultiplier;
		glassMaxVelocity[myParticleIndex] = outMaxGlassVelocity;
		numOfFluidNeighbours[myParticleIndex] = outNumOfNeighbours;
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