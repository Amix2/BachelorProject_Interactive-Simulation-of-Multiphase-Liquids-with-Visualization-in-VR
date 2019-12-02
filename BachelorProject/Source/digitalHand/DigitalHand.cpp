#include "DigitalHand.h"




DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader) 
	: m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader { handShader }
{
	//m_pyramid = std::make_unique<PyramidPointerMaterialObject>(handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 }, this);
}

DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader, VR::VRGLInterop* vrglinterop) 
	: m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader{ handShader }
{
	this->m_vrglinterop = vrglinterop;
	//m_pyramid = std::make_unique<PyramidPointerMaterialObject>(handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 }, this);
}

void DigitalHand::init()
{
	m_pyramid = std::make_unique<PyramidPointerMaterialObject>(m_handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 }, this);
	m_pyramid->init();
}

void DigitalHand::load(const glm::mat4& view, const glm::mat4& projection) const
{
	if(this->getRender())
		m_pyramid->load(view, projection);
}

glm::mat4 DigitalHand::getModel() const
{
	//return SCALE_MATRIX * getMyHandMatrix() * glm::rotate(glm::scale(glm::mat4{ 1.0f }, { 0.05, 0.05, 0.2 }), -glm::pi<float>() / 2, { 1, 0, 0 });
	//if(m_selectedObject != nullptr) return *(m_selectedObject->getMatrix()) * glm::scale(glm::mat4{ 1.0f }, { 5,5,5 });
	return  m_handMatrix * glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2, glm::vec3(1, 0, 0)) * glm::scale(glm::mat4{ 1.0f }, { 5,5,5 });
}

void DigitalHand::handleKeyPress(int key, KeyState state, float deltaTime)
{
}

void DigitalHand::update()
{
	if (m_deviceIndex == -1) {
		m_vrglinterop->VrInput->DetectControllers();
		if (m_hand == LEFT_HAND) m_deviceIndex = m_vrglinterop->VrInput->GetDetectedControllers().first;
		else m_deviceIndex = m_vrglinterop->VrInput->GetDetectedControllers().second;

		if (m_deviceIndex == -1) return;
	}

	m_handMatrix = getMyHandMatrix();
	Utils::setPosition(&m_handMatrix, Utils::getPosition(m_handMatrix) * 100.0f);
	Utils::setForward(&m_handMatrix, Utils::getForward(m_handMatrix) * -(1.0f));
	//LOG_F(WARNING, "%s", glm::to_string(m_handMatrix).c_str());
	//m_handMatrix = m_handMatrix * glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 2, glm::vec3(1,0,0));
	// update matrix for hand
	// load() function will draw it with this matrix

	if (m_selectedObject == nullptr)  tryGrabDistance();
	else teleportObjectToHand();

}

glm::mat4 DigitalHand::getMyHandMatrix() const
{

	switch (m_hand) {
	case LEFT_HAND:
		return VR::openvr_m34_to_mat4(m_vrglinterop->VrGeometry->TrackedDevicePoses[3].mDeviceToAbsoluteTracking);
		return m_dataProvider->getLeftHandMatrix();
	case RIGHT_HAND:
		return VR::openvr_m34_to_mat4(m_vrglinterop->VrGeometry->TrackedDevicePoses[4].mDeviceToAbsoluteTracking);
		return m_dataProvider->getRightHandMatrix();
	}
	return glm::mat4();
}

bool DigitalHand::tryGrabDistance()
{
	if (m_selectedObject != nullptr) throw "Grab with item in hand";
	const glm::vec3 myPosition = Utils::getPosition(m_handMatrix);
	float selectedObjDistance = PROXY_GRAB_DISTANCE + 10000.f;

	for (SelectableObject* object : SelectableObjectManager::m_selectableObjects) {
		if (!object->canBeSelected()) continue;
		const glm::vec3 objPosition = Utils::getPosition(*(object->getMatrix()));
		const float distance = glm::distance(objPosition, myPosition);
	
		if (distance - object->getSelectingRadius() < PROXY_GRAB_DISTANCE) {
			if (distance < selectedObjDistance) {
				m_selectedObject = object;
				selectedObjDistance = distance;
			}
		}
	}

	if (m_selectedObject != nullptr) {
		// selected
		grab(m_selectedObject);
		return true;
	}
	else {
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
		//LOG_F(WARNING, "angle %f\n%s\n%s", angle, glm::to_string(myForward).c_str(), glm::to_string(objPosition - myPosition).c_str());
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
		grab(m_selectedObject);
		return true;
	}
	else {
		//LOG_F(WARNING, "NNNOOOTTT--- SELECTED");
		return false;
	}
}

void DigitalHand::grab(SelectableObject* object)
{
	setGrabMatrixOffset(object);
	m_selectedObject = object;
	m_selectedActionController = object->getVRActionController();
	object->grab();
}

void DigitalHand::release()
{
	m_selectedObject->release();
	m_selectedObject = nullptr;
	m_selectedActionController = nullptr;

}

void DigitalHand::teleportObjectToHand()
{
	glm::mat4 upMatrix = glm::mat4(1);
	Utils::setPosition(&upMatrix, Utils::getPosition(m_handMatrix));
	glm::mat4 offsetMatrix = glm::inverse(m_handMatrix) * upMatrix; 
	m_selectedActionController->handMovement(m_handMatrix, offsetMatrix); // m_handMatrix * offsetMatrix = m_handMatrix * m_handMatrix^-1 * upMatrix = upMatrix
	SelectableObjectManager::m_selectableObjects;
}

void DigitalHand::moveObjectWithHand()
{
	m_selectedActionController->handMovement(m_handMatrix, m_grabMatrixOffset);
}

void DigitalHand::setGrabMatrixOffset(SelectableObject* object)
{
	m_grabMatrixOffset = glm::inverse(m_handMatrix) * *(object->getMatrix());
}
