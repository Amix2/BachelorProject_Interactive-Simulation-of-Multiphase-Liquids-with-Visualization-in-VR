#include "GlassObject.h"

GlassObject::GlassObject(const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram, const ParticleObject* glass)
	: shaderProgram{ shaderProgram }
	, selectedProgram{ selectedProgram }
	, owner{ glass }

{
	generateMesh(*glass);
}

void GlassObject::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	shaderProgram.use();
	shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);
}

void GlassObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	if (!owner->getRender()) return;
	if (owner->isSelected()) {
		glStencilMask(0xFF);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	shaderProgram.use();
	shaderProgram.setUniformVariable("VP", projection * view);
	shaderProgram.setUniformVariable("M", owner->getModel());
	shaderProgram.setUniformVariable("lightDir", -glm::vec4(1, 0.8f, 1, 0));

	glDrawArrays(GL_TRIANGLES, 0, verticesSize);

	if (owner->isSelected()) {

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);

		selectedProgram.use();
		selectedProgram.setUniformVariable("MVP", projection * view * glm::scale(owner->getModel(), glm::vec3{ 1.01f }));

		glDrawArrays(GL_TRIANGLES, 0, verticesSize / 6);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

	}
}

bool GlassObject::getRender() const
{
	return owner->getRender(); 
}

void GlassObject::generateMesh(const ParticleObject& glass)
{	
	int layerSize = (glass.m_innerRadius + glass.m_thickness) * 6;
	int vertexPositionsSize = 3 * (4 * (layerSize) + 2);
	int indicesSize = 3 * (1 + 2 + 2 + 2 + 1) * layerSize;
	verticesSize = indicesSize * 6;
	int v_index = 0;
	int i_index = 0;
	vertices = new GLfloat[verticesSize];
	GLfloat* vertexPositions = new GLfloat[vertexPositionsSize];

	glm::vec3 cursor = glm::vec3{ 0.0f };
	cursor.y -= glass.m_height * 0.5;
	addVertex(cursor, v_index, vertexPositions);
	cursor.y -= glass.m_thickness;
	addVertex(cursor, v_index, vertexPositions);
	
	cursor = glm::vec3{ 0.0f };
	glm::vec2 inner{ glass.m_innerRadius, 0.0f };
	glm::vec2 outer{ glass.m_innerRadius + glass.m_thickness, 0.0f };
	for (int i = 0; i < layerSize; i++) {
		addVertex(cursor + glm::vec3{ inner.x, -glass.m_height * 0.5, inner.y }, v_index, vertexPositions);
		addVertex(cursor + glm::vec3{ inner.x, glass.m_height * 0.5, inner.y }, v_index, vertexPositions);
		addVertex(cursor + glm::vec3{ outer.x, glass.m_height * 0.5, outer.y }, v_index, vertexPositions);
		addVertex(cursor + glm::vec3{ outer.x, -glass.m_height * 0.5 -glass.m_thickness, outer.y }, v_index, vertexPositions);

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
		addTriangle(0, 2 + 4 * i, 2 + 4 * (i + 1), i_index, vertexPositions);
		addQuad(2 + 4 * i, 2 + 4 * (i + 1), 3 + 4 * i, 3 + 4 * (i + 1), i_index, vertexPositions);
		addQuad(3 + 4 * i, 3 + 4 * (i + 1), 4 + 4 * i, 4 + 4 * (i + 1), i_index, vertexPositions);
		addQuad(4 + 4 * i, 4 + 4 * (i + 1), 5 + 4 * i, 5 + 4 * (i + 1), i_index, vertexPositions);
		addTriangle(1, 5 + 4 * (i + 1), 5 + 4 * i , i_index, vertexPositions);
	}
	addTriangle(0, 2 + 4 * (layerSize - 1), 2, i_index, vertexPositions);
	addQuad(2 + 4 * (layerSize - 1), 2, 3 + 4 * (layerSize - 1), 3, i_index, vertexPositions);
	addQuad(3 + 4 * (layerSize - 1), 3, 4 + 4 * (layerSize - 1), 4, i_index, vertexPositions);
	addQuad(4 + 4 * (layerSize - 1), 4, 5 + 4 * (layerSize - 1), 5, i_index, vertexPositions);
	addTriangle(1, 5, 5 + 4 * (layerSize - 1), i_index, vertexPositions);
}

void GlassObject::addVertex(const glm::vec3& vertex, int& currentIndex, GLfloat vertexPositions[])
{
	vertexPositions[currentIndex++] = vertex.x;
	vertexPositions[currentIndex++] = vertex.y;
	vertexPositions[currentIndex++] = vertex.z;
}

void GlassObject::addTriangle(int a, int b, int c, int& currentIndex, GLfloat vertexPositions[])
{
	glm::vec3 ab = glm::vec3{ vertexPositions[b * 3] - vertexPositions[a * 3], vertexPositions[b * 3 + 1] - vertexPositions[a * 3 + 1], vertexPositions[b * 3 + 2] - vertexPositions[a * 3 + 2] };
	glm::vec3 ac = glm::vec3{ vertexPositions[c * 3] - vertexPositions[a * 3], vertexPositions[c * 3 + 1] - vertexPositions[a * 3 + 1], vertexPositions[c * 3 + 2] - vertexPositions[a * 3 + 2] };
	glm::vec3 normalVector = glm::normalize(glm::cross(ab, ac));

	vertices[currentIndex++] = vertexPositions[a * 3];
	vertices[currentIndex++] = vertexPositions[a * 3 + 1];
	vertices[currentIndex++] = vertexPositions[a * 3 + 2];
	vertices[currentIndex++] = normalVector.x;
	vertices[currentIndex++] = normalVector.y;
	vertices[currentIndex++] = normalVector.z;
	vertices[currentIndex++] = vertexPositions[b * 3];
	vertices[currentIndex++] = vertexPositions[b * 3 + 1];
	vertices[currentIndex++] = vertexPositions[b * 3 + 2];
	vertices[currentIndex++] = normalVector.x;
	vertices[currentIndex++] = normalVector.y;
	vertices[currentIndex++] = normalVector.z;
	vertices[currentIndex++] = vertexPositions[c * 3];
	vertices[currentIndex++] = vertexPositions[c * 3 + 1];
	vertices[currentIndex++] = vertexPositions[c * 3 + 2];
	vertices[currentIndex++] = normalVector.x;
	vertices[currentIndex++] = normalVector.y;
	vertices[currentIndex++] = normalVector.z;
}

void GlassObject::addQuad(int a, int b, int c, int d, int& currentIndex, GLfloat vertexPositions[])
{
	addTriangle(a, c, b, currentIndex, vertexPositions);
	addTriangle(b, c, d, currentIndex, vertexPositions);
}


