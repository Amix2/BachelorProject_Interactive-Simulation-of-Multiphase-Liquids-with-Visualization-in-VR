#pragma once

#include <map>

#include "MaterialObject.h"

class PyramidPointerMaterialObject final : MaterialObject {
public:
	PyramidPointerMaterialObject(ShaderProgram ShaderProgram, float PyramidColor[3], unsigned int BaseVertices, unsigned int PyramidHeight, unsigned int PyramidRadius)
		: MaterialObject{ ShaderProgram }, BaseVertices{ BaseVertices }, PyramidHeight{ PyramidHeight }, PyramidRadius{ PyramidRadius } {
		memcpy(this->PyramidColor, PyramidColor, sizeof(PyramidColor));
		this->Vertices = new float[(BaseVertices + 1) * 3]; // create room in memory for n base vertices + top vertex, all multiplied by 3 (coordinates)
	}
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;

protected:
	//

private:
	bool InitializeBufferObjects();
	bool InitializeVertices();
	std::map<std::string, unsigned int> BufferObjects{};
	float PyramidColor[3];
	unsigned int BaseVertices;
	unsigned int PyramidHeight;
	unsigned int PyramidRadius;

	float *Vertices = nullptr;

	float* CalculateBaseVertexCoordinateForIndex(unsigned int Index, unsigned int BaseVertices, unsigned int PyramidRadius, unsigned int PyramidHeight);

	~PyramidPointerMaterialObject(); 
};
