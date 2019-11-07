#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aVec;


out VS_OUT {
    vec4 vector;
	int objectDetailsIndex;
} vs_out;

void main()
{
    gl_Position = vec4(aPos, 1.0); 
 	vs_out.vector = vec4(aVec.x, aVec.y, aVec.z, 0.0);
	vs_out.objectDetailsIndex = floatBitsToInt(aVec.w);
}