#include "MaterialObject.h"
#include <utilities/shader/ShaderProgram.h>
#include <dataStructures/GpuResources.h>
#include <Configuration.h>
#include <dataStructures/ParticleData.h>
#include <window/listener/KeyPressListener.h>
#include <window/Window.h>

class NormalVectorsObject
	: public MaterialObject,
	public KeyPressListener
{
public:
	NormalVectorsObject(Window& window, ShaderProgram program, glm::vec4 background);
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
	void handleKeyPress(Key key) override;
private:
	unsigned int VAO;
	unsigned int VBO;
	glm::vec4 background;
	int render = 0;
};