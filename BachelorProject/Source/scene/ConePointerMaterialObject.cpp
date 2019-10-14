#include "ConePointerMaterialObject.h"

bool PyramidPointerMaterialObject::InitializeBufferObjects() {
	unsigned int VertexArrayObject{ 0 };
	unsigned int VertexBufferObject{ 0 };

	this->BufferObjects["VAO"] = VertexArrayObject;
	this->BufferObjects["VBO"] = VertexBufferObject;

	return true;
}

bool PyramidPointerMaterialObject::InitializeVertices() {
	if (this->Vertices == nullptr) {
		return false;
	}

	// Initialize top vertex
	memset(this->Vertices, 0, 3 * sizeof(float));
	memcpy(this->Vertices + 3, this->PyramidColor, sizeof(this->PyramidColor));
	// Initialize bottom vertices
	for (unsigned int Index = 1; Index <= this->BaseVertices; ++Index) {
		unsigned int FirstVertexCoordinateIndex = 6 * Index;
		unsigned int FirstVertexColorIndex = 6 * Index + 3;
		float *BaseVertexCoordinate = this->CalculateBaseVertexCoordinateForIndex(Index, this->BaseVertices, this->PyramidRadius, this->PyramidHeight);
		memcpy(this->Vertices + FirstVertexCoordinateIndex, BaseVertexCoordinate, sizeof(BaseVertexCoordinate));
		memcpy(this->Vertices + FirstVertexColorIndex, this->PyramidColor, sizeof(this->PyramidColor));

		delete[] BaseVertexCoordinate;
	}

	return true;
}

float* PyramidPointerMaterialObject::CalculateBaseVertexCoordinateForIndex(unsigned int Index, unsigned int BaseVertices, unsigned int PyramidRadius, unsigned int PyramidHeight) {
	float* VertexCoordinates = new float[3];
	if (VertexCoordinates == nullptr) {
		return nullptr;
	}

	//

	return VertexCoordinates;
}

void PyramidPointerMaterialObject::init() {
	this->InitializeBufferObjects();
	this->InitializeVertices();
}

void PyramidPointerMaterialObject::load(const glm::mat4& view, const glm::mat4& projection) const {
	//
}

PyramidPointerMaterialObject::~PyramidPointerMaterialObject() {
	delete[] this->Vertices;
}
