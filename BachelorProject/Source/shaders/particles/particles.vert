#version 430 core

layout (location = 0) in vec2 quadOffset;
layout (location = 1) in vec2 quadUV;
layout (location = 2) in vec4 particleData;

flat out int type;
out vec2 UV;
out vec3 cameraSpaceLightDir;	
out mat4 perspectiveRotation;

uniform mat4 view;
uniform mat4 projection;
uniform float particleSize;
uniform vec4 lightDir;

mat4 translate(float x, float y, float z);
mat4 rotateFromAToB(vec4 A, vec4 B);
mat4 rotateZ(float angle);

void main()
{	
	mat4 MV =  view * translate(particleData.x, particleData.y, particleData.z);
	MV[0][0] = 1.0; 
	MV[0][1] = 0.0; 
	MV[0][2] = 0.0; 
	MV[1][0] = 0.0; 
	MV[1][1] = 1.0; 
	MV[1][2] = 0.0; 
	MV[2][0] = 0.0; 
	MV[2][1] = 0.0; 
	MV[2][2] = 1.0; 

	cameraSpaceLightDir = normalize(vec3(view * lightDir));
	perspectiveRotation = rotateFromAToB(vec4(0,0,-1,0), normalize(vec4(MV[3][0], MV[3][1], MV[3][2], 0))) * 
	rotateZ(2 * atan(cameraSpaceLightDir.y / cameraSpaceLightDir.x));
	vec4 position = projection * MV * vec4(quadOffset * particleSize, 0.0, 1.0);
	if(position.x * position.y * position.z == 0)
		type = 0;
	else 
		type = floatBitsToInt(particleData.w);
	UV = quadUV;
	gl_Position =  position;
}

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

mat4 rotateFromAToB(vec4 A, vec4 B) {
	vec3 crossAB = cross(vec3(A), vec3(B));
	vec4 a = vec4(crossAB / length(crossAB), 0);
	float angle = acos(dot(vec3(A), vec3(B)));
	float s = sin(angle);
	float c = cos(angle);
	return mat4(
		vec4(a.x * a.x * (1 - c) + c, a.x * a.y * (1 - c) - s * a.z, a.x * a.z * (1 - c) - s * a.y, 0),
		vec4(a.x * a.y * (1 - c) - s * a.z, a.y * a.y * (1 - c) + c, a.y * a.z * (1 - c) + s * a.x, 0),
		vec4(a.x * a.z * (1 - c) + s * a.y , a.y * a.z * (1 - c) - s * a.x, a.z * a.z * (1 - c) + c, 0),
		vec4(0, 0, 0, 1)
	);
}

mat4 rotateZ(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	return mat4(
		vec4(c, s, 0, 0),
		vec4(-s, c, 0, 0),
		vec4(0, 0, 1, 0),
		vec4(0, 0, 0, 1)
	);
}
