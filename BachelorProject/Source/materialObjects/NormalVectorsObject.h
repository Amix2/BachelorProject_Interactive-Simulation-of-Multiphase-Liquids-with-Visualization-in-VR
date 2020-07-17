#include "MaterialObject.h"
#include <utilities/shader/ShaderProgram.h>
#include <dataStructures/GpuResources.h>
#include <Configuration.h>
#include <dataStructures/ParticleData.h>
#include <inputDispatcher/InputDispatcher.h>

class NormalVectorsObject
	: public MaterialObject,
	public KeyInputListener
{
public:
	NormalVectorsObject(InputDispatcher& inputDispatcher, ShaderProgram program);
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
	void handleKeyPress(int key, KeyState action, float deltaTime) override;
private:
	unsigned int VAO;
	unsigned int VBO;
	int render = 0;
};