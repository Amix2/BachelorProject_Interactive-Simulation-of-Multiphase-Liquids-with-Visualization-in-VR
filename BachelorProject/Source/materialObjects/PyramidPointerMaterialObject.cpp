#include "PyramidPointerMaterialObject.h"
#include <VR/VRGeometry.h>


PyramidPointerMaterialObject::PyramidPointerMaterialObject(ShaderProgram ShaderProgram, glm::vec4 PyramidColor, const MaterialObjectOwner* owner)
	: MaterialObject{ ShaderProgram }, PyramidColor{ PyramidColor }, m_owner{ owner }
{
}

bool PyramidPointerMaterialObject::InitializeBufferObjects() {
	GLuint VertexArrayObject{};
	GLuint VertexBufferObject{};
	GLuint ElementBufferObject{};

	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	glGenBuffers(1, &VertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);

	glGenBuffers(1, &ElementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), this->Vertices, GL_DYNAMIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(GLuint), this->Elements, GL_DYNAMIC_DRAW);

	this->VAO = VertexArrayObject;
	this->VBO = VertexBufferObject;
	this->EBO = ElementBufferObject;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return true;
}

//bool PyramidPointerMaterialObject::InitializeVertices() {
//	if (this->Vertices == nullptr) {
//		return false;
//	}
//
//	// Initialize top vertex
//	memset(this->Vertices, 0, 3 * sizeof(float));
//	//memcpy(this->Vertices + 3, this->PyramidColor, sizeof(this->PyramidColor));
//	// Initialize bottom vertices
//	for (unsigned int Index = 1; Index <= this->BaseVertices; ++Index) {
//		unsigned int FirstVertexCoordinateIndex = 6 * Index;
//		unsigned int FirstVertexColorIndex = 6 * Index + 3;
//		float *BaseVertexCoordinate = this->CalculateBaseVertexCoordinateForIndex(Index, this->PyramidRadius, this->PyramidHeight);
//		memcpy(this->Vertices + FirstVertexCoordinateIndex, BaseVertexCoordinate, sizeof(BaseVertexCoordinate));
//		//memcpy(this->Vertices + FirstVertexColorIndex, this->PyramidColor, sizeof(this->PyramidColor));
//
//		delete[] BaseVertexCoordinate;
//	}
//
//	return true;
//}

//glm::vec3 PyramidPointerMaterialObject::CalculateBaseVertexCoordinateForIndex(unsigned int Index, double PyramidRadius, double PyramidHeight) const {
//	glm::vec3 VertexCoordinates{};
//	//
//	
//	return VertexCoordinates;
//}
//
//glm::vec3 PyramidPointerMaterialObject::CalculateBaseVertexPosition(double PyramidRadius, double PyramidHeight) {
//	glm::vec3 
//	return glm::vec3{};
//}

void PyramidPointerMaterialObject::init() {
	this->InitializeBufferObjects();
	//this->InitializeVertices();
	this->shaderProgram.use();
	this->shaderProgram.setUniformVariable("pyramidColor", this->PyramidColor);
	this->shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);
}

void PyramidPointerMaterialObject::load(const glm::mat4& view, const glm::mat4& projection) const {
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	vr::TrackedDevicePose_t trackedDevicePose;
	vr::VRControllerState_t controllerState;
	int deviceId;
	this->shaderProgram.use();

	this->shaderProgram.setUniformVariable("MVP", projection * view * m_owner->getModel());

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

}
