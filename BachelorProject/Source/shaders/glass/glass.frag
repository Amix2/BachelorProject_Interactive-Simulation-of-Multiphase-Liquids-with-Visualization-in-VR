#version 330 core
out vec4 FragColor;


float near = 0.1;
float far = 1000.0;

uniform vec4 background;
in float diffuse;


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	


	float depth = LinearizeDepth(gl_FragCoord.z) / far; 
	vec4 tmp = vec4(0.3, 0.3, 0.3, 1.0);
	tmp.x *= 0.5;
	tmp.y *= 0.5;
	tmp.z *= 0.5;
	tmp.x += 0.40;
	tmp.y += 0.25;
	tmp.z += 0.15;
	tmp *= (diffuse * 0.6 + 0.4);


	FragColor = vec4(
		mix(tmp.x, background.r, depth),
		mix(tmp.y, background.g, depth),
		mix(tmp.z, background.b, depth),
		1.0
	);
}