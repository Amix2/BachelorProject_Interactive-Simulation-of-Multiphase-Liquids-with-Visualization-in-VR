#version 330 core
in vec4 color;
out vec4 FragColor;

float near = 0.1;
float far = 1000.0;

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
		mix(color.r, background.r, depth),
		mix(color.g, background.g, depth),
		mix(color.b, background.b, depth),
		1.0
	);
}