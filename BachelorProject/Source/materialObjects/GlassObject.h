#pragma once
#include <particleObjects/ParticleObject.h>
#include <vector>
#include <glm/gtx/rotate_vector.hpp>
#include <Configuration.h>
class ParticleObject;

class GlassObject : public MaterialObject
{
public:
	GlassObject(const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram, const ParticleObject* glass);
	void init();
	void load(const glm::mat4& view, const glm::mat4& projection) const;

	bool getRender() const;
	void toggleRender() = delete;
	
private:
	const ShaderProgram selectedProgram;
	ShaderProgram shaderProgram;

	const ParticleObject* owner;

	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	GLfloat* vertices;
	GLint* indices;
	int verticesSize;
	int indicesSize;


	void generateMesh(const ParticleObject& glass);
	void addVertex(const glm::vec3& vertex, int& currentIndex);
	void addTriangle(int a, int b, int c, int& currentIndex);
	void addQuad(int a, int b, int c, int d, int& currentIndex);
};

