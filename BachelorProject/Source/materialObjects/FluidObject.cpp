#include "FluidObject.h"
#include <Logger.h>

FluidObject::FluidObject(const ShaderProgram& shaderProgram)
	: MaterialObject{ shaderProgram } {}

void FluidObject::init()
{

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	int index = GpuResources::getIndex(BufferDatails::particlePositionsName);
	glBindBuffer(GL_ARRAY_BUFFER, index);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	int width;
	int height;
	int nrChannels;
	unsigned char* data = stbi_load("./_asset/ball.png", &width, &height, &nrChannels, 0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float textureBorderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //transparent
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	} 
	stbi_image_free(data);

}

void FluidObject::load(const glm::mat4& view, const glm::mat4& projection) const
{


	int index = GpuResources::getIndex(BufferDatails::particlePositionsName);
	glBindBuffer(GL_ARRAY_BUFFER, index);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textureID);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0,10);
	glDisable(GL_BLEND);
}

