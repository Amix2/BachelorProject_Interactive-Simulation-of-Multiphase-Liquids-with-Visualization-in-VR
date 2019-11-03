#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;
in float type;

uniform sampler2D ourTexture;
uniform vec4 background;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	vec4 tmp = texture(ourTexture, texCoord);
	if(type > 0)
		tmp.z += 0.5;
	else
		tmp.x += 0.2;

	if(tmp.a < 1)
		discard;
	FragColor = vec4(
		mix(tmp.r, background.r, depth),
		mix(tmp.g, background.g, depth),
		mix(tmp.b, background.b, depth),
		1.0
	);
}