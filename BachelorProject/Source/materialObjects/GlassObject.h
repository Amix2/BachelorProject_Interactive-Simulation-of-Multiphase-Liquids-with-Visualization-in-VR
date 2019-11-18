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
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
	void select(bool flag) { selected = flag; }
	void select() { selected = !selected; }
	
private:
	const ShaderProgram selectedProgram;

	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	const glm::mat4* model;

	GLfloat* vertices;
	GLint* indices;
	int verticesSize;
	int indicesSize;


	bool selected = false;

	void generateMesh(const ParticleObject& glass);
	void addVertex(const glm::vec3& vertex, int& currentIndex);
	void addTriangle(int a, int b, int c, int& currentIndex);
	void addQuad(int a, int b, int c, int d, int& currentIndex);
};

