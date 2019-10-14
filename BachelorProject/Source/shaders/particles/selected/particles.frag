#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;
uniform sampler2D ourTexture;

void main()
{
	vec4 tmp = texture(ourTexture, texCoord);
	if(tmp.a < 1)
		discard;

	FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}