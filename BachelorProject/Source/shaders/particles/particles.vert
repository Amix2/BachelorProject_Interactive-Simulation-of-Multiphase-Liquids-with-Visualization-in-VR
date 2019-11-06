#version 430 core
layout (location = 0) in vec4 aPos;



out VS_OUT {
	int type;
} vs_out;

void main()
{	
	vs_out.type = floatBitsToInt(aPos.w);
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); 

}