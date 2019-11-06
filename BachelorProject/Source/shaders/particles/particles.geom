#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;
uniform float particleSize;

out vec2 texCoord;
flat out int type;

in VS_OUT {
	int type;
} gs_in[];


mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

void buildBilboard(vec4 position)
{    
	// FIX this if there is a better way to ignore points with this condition 
	if(position.x * position.y * position.z == 0) return;

	mat4 modelView = view * translate(position.x, position.y, position.z);
	modelView[0][0] = 1.0; 
	modelView[0][1] = 0.0; 
	modelView[0][2] = 0.0; 
	modelView[1][0] = 0.0; 
	modelView[1][1] = 1.0; 
	modelView[1][2] = 0.0; 
	modelView[2][0] = 0.0; 
	modelView[2][1] = 0.0; 
	modelView[2][2] = 1.0; 

	type = gs_in[0].type;
    texCoord = vec2(0.0, 0.0);
    gl_Position = projection * modelView * (vec4(-particleSize, -particleSize, 0.0, 1.0)); // 1:bottom-left   
    EmitVertex();   
	texCoord = vec2(1.0, 0.0);
    gl_Position = projection * modelView *  (vec4( particleSize, -particleSize, 0.0, 1.0)); // 2:bottom-right
    EmitVertex();
	texCoord = vec2(0.0, 1.0);
    gl_Position =  projection * modelView *  (vec4(-particleSize, particleSize, 0.0, 1.0)); // 3:top-left
    EmitVertex();
	texCoord = vec2(1.0, 1.0);
    gl_Position = projection * modelView *  (vec4( particleSize, particleSize, 0.0, 1.0)); // 4:top-right
    EmitVertex();

    EndPrimitive();
}

void main() {
	if(gs_in[0].type > 0)
		buildBilboard(gl_in[0].gl_Position);
}