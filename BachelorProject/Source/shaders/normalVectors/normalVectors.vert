#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aVec;
layout (location = 2) in int aIndex;

out VS_OUT {
    vec4 vector;
	int objectDetailsIndex;
} vs_out;

void main()
{
    gl_Position = vec4(aPos, 1.0); 
	vs_out.vector = vec4(aVec, 1.0);
	vs_out.objectDetailsIndex = aIndex;
}