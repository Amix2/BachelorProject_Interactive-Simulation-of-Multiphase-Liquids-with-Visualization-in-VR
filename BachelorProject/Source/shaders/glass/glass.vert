#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


uniform mat4 M;
uniform mat4 VP;
uniform vec4 lightDir;

out float diffuse;

void main()
{
	
	gl_Position = VP * M * vec4(aPos, 1.0);
	diffuse = min(max(dot(normalize(vec3(M * vec4(aNormal, 0))), normalize(vec3(lightDir))), 0 ), 1);
}