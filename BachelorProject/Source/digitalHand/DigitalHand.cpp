#include "DigitalHand.h"

DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand) : m_dataProvider{ dataprovider }, m_hand{ hand }
{
	m_pyramid = std::make_shared<PyramidPointerMaterialObject>();
}

void DigitalHand::init()
{
}

void DigitalHand::load(const glm::mat4& view, const glm::mat4& projection) const
{
}

void DigitalHand::handleKeyPress(int key, KeyState state, float deltaTime)
{
}

void DigitalHand::update()
{
}
