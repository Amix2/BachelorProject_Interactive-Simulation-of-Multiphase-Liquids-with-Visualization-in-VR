#include "DigitalHand.h"




DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader) 
	: m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader { handShader }
{
	m_pyramid = std::make_unique<PyramidPointerMaterialObject>(handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 }, this);
}

DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader, const VR::VRGLInterop& vrglinterop) 
	: m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader{ handShader }
{
	m_pyramid = std::make_unique<PyramidPointerMaterialObject>(handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 }, this);
}

void DigitalHand::init()
{
	m_pyramid->init();
}

void DigitalHand::load(const glm::mat4& view, const glm::mat4& projection) const
{
	m_pyramid->load(view, projection);
}

const glm::mat4 DigitalHand::getPyramidModel() const
{
	return m_handMatrix
		* glm::rotate(glm::scale(glm::mat4{ 1.0f }, { 0.05, 0.05, 0.2 }), -glm::pi<float>() / 2, { 1, 0, 0 });
}

void DigitalHand::handleKeyPress(int key, KeyState state, float deltaTime)
{
}

void DigitalHand::update()
{
	m_handMatrix = SCALE_MATRIX * getMyHandMatrix();
	// update matrix for hand
	// load() function will draw it with this matrix
	// if hand to holding st -> use SelectableObjectManager to move object
	if (m_selectedObject == nullptr)  tryGrab();
	else teleportObjectToHand(m_selectedObject);
}

glm::mat4 DigitalHand::getMyHandMatrix() const
{
	switch (m_hand) {
	case LEFT_HAND:
		return m_dataProvider->getLeftHandMatrix();
	case RIGHT_HAND:
		return m_dataProvider->getRightHandMatrix();
	}
	return glm::mat4();
}

bool DigitalHand::tryGrab()
{
	if (m_selectedObject != nullptr) throw "Grab with item in hand";
	const glm::vec3 myPosition = Utils::getPosition(m_handMatrix);
	float selectedObjDistance = PROXY_GRAB_DISTANCE;
	for (SelectableObject* object : SelectableObjectManager::m_selectableObjects) {
		if (!object->canBeSelected()) continue;
		const glm::vec3 objPosition = Utils::getPosition(*(object->getMatrix()));
		if (glm::distance(objPosition, myPosition) < selectedObjDistance) {
			m_selectedObject = object;
		}
	}
	if (m_selectedObject != nullptr) {
		// selected
		//LOG_F(WARNING, "SELECTED");
		m_selectedObject->grab();
		return true;
	}
	else {
		//LOG_F(WARNING, "NNNOOOTTT--- SELECTED");
		return false;
	}
}

void DigitalHand::teleportObjectToHand(SelectableObject* obj)
{
	obj->setMatrix(m_handMatrix);
}


