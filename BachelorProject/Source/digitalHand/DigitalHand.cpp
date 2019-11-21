#include "DigitalHand.h"




DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader) : m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader { handShader }
{
	m_pyramid = std::make_shared<PyramidPointerMaterialObject>(handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 });
}

DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader, const VR::VRGLInterop& vrglinterop) : m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader{ handShader }
{
	m_pyramid = std::make_shared<PyramidPointerMaterialObject>(handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 }, vrglinterop);
}

void DigitalHand::init()
{
	m_pyramid->init();
}

void DigitalHand::load(const glm::mat4& view, const glm::mat4& projection) const
{
	m_pyramid->load(view, projection);
}

void DigitalHand::handleKeyPress(int key, KeyState state, float deltaTime)
{
}

void DigitalHand::update()
{
	const glm::mat4 handMatrix = getMyHandMatrix();
	// update matrix for hand
	// load() function will draw it with this matrix
	// if hand to holding st -> use SelectableObjectManager to move object

}

glm::mat4 DigitalHand::getMyHandMatrix()
{
	switch (m_hand) {
	case LEFT_HAND:
		return m_dataProvider->getLeftHandMatrix();
	case RIGHT_HAND:
		return m_dataProvider->getRightHandMatrix();
	}
	return glm::mat4();
}