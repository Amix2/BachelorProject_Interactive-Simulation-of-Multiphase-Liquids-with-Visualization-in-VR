#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;

uniform sampler2D ourTexture;

float near = 0.1; 
float far  = 25.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	vec4 tmp = texture(ourTexture, texCoord);
	tmp.x += depth;
	tmp.y += depth;
	tmp.z += depth;
	if(tmp.a < 0.001)
		discard;
    FragColor = tmp;
}