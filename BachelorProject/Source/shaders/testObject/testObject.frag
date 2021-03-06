#version 330 core
out vec4 FragColor;


float near = 0.1;
float far = 200.0;

uniform vec4 background;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far; 
	FragColor = vec4(
		mix(1.0, background.r, depth),
		mix(1.0, background.g, depth),
		mix(1.0, background.b, depth),
		1.0
	);
}