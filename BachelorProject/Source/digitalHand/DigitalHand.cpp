#include "DigitalHand.h"




DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader) 
	: m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader { handShader }
{
	m_pyramid = std::make_unique<PyramidPointerMaterialObject>(handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 }, this);
}

DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader, VR::VRGLInterop* vrglinterop) 
	: m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader{ handShader }
{
	this->vrglinterop = vrglinterop;
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
		* glm::scale(glm::mat4{ 1.0f }, { 0.05, 0.05, 0.2 });
}

void DigitalHand::handleKeyPress(int key, KeyState state, float deltaTime)
{
}

void DigitalHand::update()
{
	m_handMatrix = SCALE_MATRIX * getMyHandMatrix() * glm::rotate((glm::mat4{ 1.0f }), -glm::pi<float>(), { 1, 0, 0 });
	// update matrix for hand
	// load() function will draw it with this matrix
	// if hand to holding st -> use SelectableObjectManager to move object
	if (m_selectedObject == nullptr)  tryGrabAngle();
	else teleportObjectToHand(m_selectedObject);
}

glm::mat4 DigitalHand::getMyHandMatrix() const
{

	switch (m_hand) {
	case LEFT_HAND:
		return VR::openvr_m34_to_mat4(vrglinterop->VrGeometry->TrackedDevicePoses[3].mDeviceToAbsoluteTracking);
		return m_dataProvider->getLeftHandMatrix();
	case RIGHT_HAND:
		return VR::openvr_m34_to_mat4(vrglinterop->VrGeometry->TrackedDevicePoses[4].mDeviceToAbsoluteTracking);
		return m_dataProvider->getRightHandMatrix();
	}
	return glm::mat4();
}

bool DigitalHand::tryGrabDistance()
{
	if (m_selectedObject != nullptr) throw "Grab with item in hand";
	const glm::vec3 myPosition = Utils::getPosition(m_handMatrix);
	float selectedObjDistance = PROXY_GRAB_DISTANCE;

	for (SelectableObject* object : SelectableObjectManager::m_selectableObjects) {
		if (!object->canBeSelected()) continue;
		const glm::vec3 objPosition = Utils::getPosition(*(object->getMatrix()));
		const float distance = glm::distance(objPosition, myPosition);

		if (distance < selectedObjDistance) {
			m_selectedObject = object;
			selectedObjDistance = distance;

		}
	}

	if (m_selectedObject != nullptr) {
		// selected
		//LOG_F(WARNING, "SELECTED");
		m_selectedObject->grab();
		setGrabPositionOffset();
		return true;
	}
	else {
		//LOG_F(WARNING, "NNNOOOTTT--- SELECTED");
		return false;
	}
}

bool DigitalHand::tryGrabAngle()
{
	if (m_selectedObject != nullptr) throw "Grab with item in hand";

	const glm::vec3 myPosition = Utils::getPosition(m_handMatrix);
	float selectedObjDistance = INT_MAX;

	const glm::vec3 myForward = glm::normalize(Utils::getForward(m_handMatrix));

	for (SelectableObject* object : SelectableObjectManager::m_selectableObjects) {
		if (!object->canBeSelected()) continue;

		const glm::vec3 objPosition = Utils::getPosition(*(object->getMatrix()));
		const glm::vec3 vecToObject = glm::normalize(objPosition - myPosition);
		const float angle = std::acos(glm::dot(myForward, vecToObject));

		if (angle < ANGLE_GRAB_DIFFERENCE) {
			const float distance = glm::distance(objPosition, myPosition);
			if (distance < selectedObjDistance) {
				m_selectedObject = object;
				selectedObjDistance = distance;
			}
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
	glm::mat4 objectMatrix = *(obj->getMatrix());
	Utils::setPosition(&objectMatrix, Utils::getPosition(m_handMatrix));
	obj->setMatrix(objectMatrix);
}

void DigitalHand::setGrabPositionOffset()
{
	m_grabPositionOffset = Utils::getPosition(*(m_selectedObject->getMatrix())) - Utils::getPosition(m_handMatrix);
}


