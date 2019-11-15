#version 430 core
#define MAX_FLUID_TYPES 10

out vec4 FragColor;
  
in vec2 UV;
flat in int type;

struct FluidType {
	float mass;
	float stiffness;
	float viscosity;
	float density;
	vec4 color;
};

layout(std140, binding = 5) uniform fluidTypesBuf
{
	FluidType fluidTypeArray[MAX_FLUID_TYPES];
};


uniform sampler2D ourTexture;
uniform vec4 background;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

float avg(float a, float b) {
	return (a + b) * 0.5f;
}

void main()
{
	if(type <= 0)
		discard;
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	vec4 texturedPixel = texture(ourTexture, UV);
	const FluidType fluidType = fluidTypeArray[type];
	texturedPixel.x = avg(fluidType.color.x, texturedPixel.x);
	texturedPixel.y = avg(fluidType.color.y, texturedPixel.y);
	texturedPixel.z = avg(fluidType.color.z, texturedPixel.z);
	if(texturedPixel.a < 1.0)
		discard;

	FragColor = vec4(
		mix(texturedPixel.r, background.r, depth),
		mix(texturedPixel.g, background.g, depth),
		mix(texturedPixel.b, background.b, depth),
		1.0
	);
}