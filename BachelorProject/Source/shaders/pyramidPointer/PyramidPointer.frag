#version 330 core

out vec4 FragColorOut;

float near = 0.1;
float far = 200.0;

uniform vec4 background;
uniform vec4 pyramidColor;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far; 
	FragColorOut = vec4(
		mix(pyramidColor.r, background.r, depth),
		mix(pyramidColor.g, background.g, depth),
		mix(pyramidColor.b, background.b, depth),
		1.0
	);
}