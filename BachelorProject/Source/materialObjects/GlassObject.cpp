#include "GlassObject.h"

GlassObject::GlassObject(const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram, const ParticleObject* glass)
	: shaderProgram{ shaderProgram }
	, selectedProgram{ selectedProgram }
	, model{ &glass->m_matrix }
	, owner{ glass }

{
	generateMesh(*glass);
}

void GlassObject::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(GLint), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	shaderProgram.use();
	shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);
}

void GlassObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	if (!owner->render) return;
	if (owner->isSelected()) {
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	shaderProgram.use();
	shaderProgram.setUniformVariable("MVP", projection * view * (*model));

	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);

	if (owner->isSelected()) {
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);

		selectedProgram.use();
		selectedProgram.setUniformVariable("MVP", projection * view * glm::scale((*model), glm::vec3{ 1.01f }));

		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);

		glStencilMask(0xFF);
	}
}

void GlassObject::generateMesh(const ParticleObject& glass)
{	
	int layerSize = (glass.m_innerRadius + glass.m_thickness) * 6;
	verticesSize = 3 * 4 * (layerSize + 1) + 2;
	indicesSize = 3 * (1 + 2 + 2 + 2 + 1) * layerSize;
	int v_index = 0;
	int i_index = 0;
	vertices = new GLfloat[verticesSize];
	indices = new GLint[indicesSize];

	glm::vec3 cursor = glm::vec3{ 0.0f };
	cursor.y -= glass.m_height * 0.5;
	addVertex(cursor, v_index);
	cursor.y -= glass.m_thickness;
	addVertex(cursor, v_index);
	
	cursor = glm::vec3{ 0.0f };
	glm::vec2 inner{ glass.m_innerRadius, 0.0f };
	glm::vec2 outer{ glass.m_innerRadius + glass.m_thickness, 0.0f };
	for (int i = 0; i < layerSize + 1; i++) {
		addVertex(cursor + glm::vec3{ inner.x, -glass.m_height * 0.5, inner.y }, v_index);
		addVertex(cursor + glm::vec3{ inner.x, glass.m_height * 0.5, inner.y }, v_index);
		addVertex(cursor + glm::vec3{ outer.x, glass.m_height * 0.5, outer.y }, v_index);
		addVertex(cursor + glm::vec3{ outer.x, -glass.m_height * 0.5 -glass.m_thickness, outer.y }, v_index);

		inner = glm::rotate(inner, glm::radians(360.0f / layerSize));
		outer = glm::rotate(outer, glm::radians(360.0f / layerSize));
	}

	//layout of vertices
	//		__index__		__description__
	//			0			 central upper
	//			1			 central lower
	//      2 + 4 * i		 inner lower
	//		3 + 4 * i		 inner upper
	//		4 + 4 * i		 outer upper
	//		5 + 4 * i		 outer lower

	for (int i = 0; i < layerSize - 1; i++) {
		addTriangle(0, 2 + 4 * i, 2 + 4 * (i + 1), i_index);
		addQuad(2 + 4 * i, 2 + 4 * (i + 1), 3 + 4 * i, 3 + 4 * (i + 1), i_index);
		addQuad(3 + 4 * i, 3 + 4 * (i + 1), 4 + 4 * i, 4 + 4 * (i + 1), i_index);
		addQuad(4 + 4 * i, 4 + 4 * (i + 1), 5 + 4 * i, 5 + 4 * (i + 1), i_index);
		addTriangle(1, 5 + 4 * (i + 1), 5 + 4 * i , i_index);
	}
	addTriangle(0, 2 + 4 * (layerSize - 1), 2, i_index);
	addQuad(2 + 4 * (layerSize - 1), 2, 3 + 4 * (layerSize - 1), 3, i_index);
	addQuad(3 + 4 * (layerSize - 1), 3, 4 + 4 * (layerSize - 1), 4, i_index);
	addQuad(4 + 4 * (layerSize - 1), 4, 5 + 4 * (layerSize - 1), 5, i_index);
	addTriangle(1, 5, 5 + 4 * (layerSize - 1), i_index);
}

void GlassObject::addVertex(const glm::vec3& vertex, int& currentIndex)
{
	vertices[currentIndex++] = vertex.x;
	vertices[currentIndex++] = vertex.y;
	vertices[currentIndex++] = vertex.z;
}

void GlassObject::addTriangle(int a, int b, int c, int& currentIndex)
{
	indices[currentIndex++] = a;
	indices[currentIndex++] = b;
	indices[currentIndex++] = c;
}

void GlassObject::addQuad(int a, int b, int c, int d, int& currentIndex)
{
	addTriangle(a, c, b, currentIndex);
	addTriangle(b, c, d, currentIndex);
}


