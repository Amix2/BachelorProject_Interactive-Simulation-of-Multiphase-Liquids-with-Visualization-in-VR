#version 430 core

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

layout (location = 0) in vec2 quadOffset;
layout (location = 1) in vec2 quadUV;
layout (location = 2) in vec4 particleData;

flat out int type;
out vec2 UV;

uniform mat4 view;
uniform mat4 projection;
uniform float particleSize;

void main()
{	
	mat4 modelView = view * translate(particleData.x, particleData.y, particleData.z);
	modelView[0][0] = 1.0; 
	modelView[0][1] = 0.0; 
	modelView[0][2] = 0.0; 
	modelView[1][0] = 0.0; 
	modelView[1][1] = 1.0; 
	modelView[1][2] = 0.0; 
	modelView[2][0] = 0.0; 
	modelView[2][1] = 0.0; 
	modelView[2][2] = 1.0;
	
	vec4 position = projection * modelView * vec4(quadOffset * particleSize
	, 0.0, 1.0);
	if(position.x * position.y * position.z == 0)
		type = 0;
	else 
		type = floatBitsToInt(particleData.w);
	UV = quadUV;
	gl_Position =  position;
}