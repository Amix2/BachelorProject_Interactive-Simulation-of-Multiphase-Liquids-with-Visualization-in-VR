#version 430 core
#define MAX_FLUID_TYPES 10

out vec4 FragColor;
  
in vec2 UV;
in vec3 cameraSpaceLightDir;
flat in int type;
in mat4 rot;
in mat4 inv;

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


uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

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
	vec4 texturedPixel = texture(colorTexture, UV);
	if(texturedPixel.a < 1)
		discard;
	vec3 pixelNormal = texture(normalTexture, UV).rgb;
	pixelNormal = normalize(pixelNormal * 2.0 - 1.0);

	const FluidType fluidType = fluidTypeArray[type];
	
	float lightIntensity = min(max(dot(vec3(vec4(pixelNormal, 0)), cameraSpaceLightDir), 0), 1);

	texturedPixel.r = avg(fluidType.color.r, texturedPixel.r);
	texturedPixel.g = avg(fluidType.color.g, texturedPixel.g);
	texturedPixel.b = avg(fluidType.color.b, texturedPixel.b);
	vec3 ambient = vec3(texturedPixel);

	texturedPixel *= lightIntensity;



	FragColor = vec4(
		mix(texturedPixel.r, background.r, depth),
		mix(texturedPixel.g, background.g, depth),
		mix(texturedPixel.b, background.b, depth),
		1.0
		);
//	FragColor = vec4(
//		mix(texturedPixel.r * 0.2 + 0.8 * ambient.r, background.r, depth),
//		mix(texturedPixel.g * 0.2 * 0.8 * ambient.g, background.g, depth),
//		mix(texturedPixel.b * 0.2 + 0.8 * ambient.b, background.b, depth),
//		1.0
//		);
}