#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;
in float type;

uniform sampler2D ourTexture;

float near = 0.1; 
float far  = 1000.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	vec4 tmp = texture(ourTexture, texCoord);
	if(int(type) > 0) 
		tmp.z += 0.5;
	else {
		tmp.x *= 0.5;
		tmp.y *= 0.5;
		tmp.z *= 0.5;
		tmp.x += 0.40;
		tmp.y += 0.25;
		tmp.z += 0.15;
	}
	tmp.x += depth;
	tmp.y += depth;
	tmp.z += depth;
	if(tmp.a < 1)
		discard;
    FragColor = tmp;
}