#version 430 core

#define MAX_PARTICLE_OBJECTS 10

layout (points) in;
layout (line_strip, max_vertices = 2) out;

struct GlassObjectDetails {
	mat4 matrix;
};

layout(std140, binding = 3) uniform glassObjectsBuf
{
	GlassObjectDetails glassObjects[MAX_PARTICLE_OBJECTS];
};

uniform mat4 view;
uniform mat4 projection;
uniform int render;

in VS_OUT {
    vec4 vector;
	int objectDetailsIndex;
} gs_in[];

void drawVector(vec4 position)
{    
	if(!bool(render)) return;
	mat4 MVP = projection * view  * glassObjects[gs_in[0].objectDetailsIndex].matrix;
	
    gl_Position =  MVP * position;
    EmitVertex();
    gl_Position = MVP * (position + (gs_in[0].vector * 0.05));
    EmitVertex();
    EndPrimitive();
}

void main() {
	drawVector(gl_in[0].gl_Position);
}