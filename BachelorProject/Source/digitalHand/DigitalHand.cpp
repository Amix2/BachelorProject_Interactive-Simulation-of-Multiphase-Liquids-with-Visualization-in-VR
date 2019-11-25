#include "DigitalHand.h"




DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader) 
	: m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader { handShader }
{
	//m_pyramid = std::make_unique<PyramidPointerMaterialObject>(handShader, glm::vec4{ 0.3, 0.5, 0.4, 1.0 }, this);
}

DigitalHand::DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader, VR::VRGLInterop* vrglinterop) 
	: m_dataProvider{ dataprovider }, m_hand{ hand }, m_handShader{ handShader }
{
	this->vrglinterop = vrglinterop;
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
	return  m_handMatrix * glm::scale(glm::mat4{ 1.0f }, { 5,5,5 });
}

void DigitalHand::handleKeyPress(int key, KeyState state, float deltaTime)
{
}

void DigitalHand::update()
{
	m_handMatrix = getMyHandMatrix();
	Utils::setPosition(&m_handMatrix, Utils::getPosition(m_handMatrix) * 100.0f);
	m_handMatrix = m_handMatrix * glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 2, glm::vec3(1,0,0));
	//LOG_F(WARNING, "%s", glm::to_string(m_handMatrix).c_str());
	// update matrix for hand
	// load() function will draw it with this matrix
	// if hand to holding st -> use SelectableObjectManager to move object
	if (m_selectedObject == nullptr)  tryGrabDistance();
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
		setGrabMatrixOffset();
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
	//`glm::mat4 objectMatrix = *(obj->getMatrix());
	//Utils::setPosition(&objectMatrix, Utils::getPosition(m_handMatrix));
	glm::mat4 objMatrix = m_handMatrix * m_grabMatrixOffset;
	//m_matrixDifference.applyDifference(m_handMatrix, &objMatrix);
	obj->setMatrix(objMatrix);
}

void DigitalHand::setGrabMatrixOffset()
{
	m_grabMatrixOffset = glm::inverse(m_handMatrix) * *(m_selectedObject->getMatrix());
	//m_matrixDifference.createDifference(m_handMatrix, *(m_selectedObject->getMatrix()));
	//m_grabPositionOffset = Utils::getPosition(*(m_selectedObject->getMatrix())) - Utils::getPosition(m_handMatrix);
}

void MatrixDifference::createDifference(const glm::mat4& refMatrix, const glm::mat4& otherMatrix)
{
	angForwardByRight = glm::orientedAngle(Utils::getForward(refMatrix), Utils::getForward(otherMatrix), Utils::getRight(refMatrix));
	angForwardByUp = glm::orientedAngle(Utils::getForward(refMatrix), Utils::getForward(otherMatrix), Utils::getUp(refMatrix));

	angUpByForward = glm::orientedAngle(Utils::getUp(refMatrix), Utils::getUp(otherMatrix), Utils::getForward(refMatrix));
	angUpByRight = glm::orientedAngle(Utils::getUp(refMatrix), Utils::getUp(otherMatrix), Utils::getRight(refMatrix));

	angRightByForward = glm::orientedAngle(Utils::getRight(refMatrix), Utils::getRight(otherMatrix), Utils::getForward(refMatrix));
	angRightByUp = glm::orientedAngle(Utils::getRight(refMatrix), Utils::getRight(otherMatrix), Utils::getUp(refMatrix));
}

void MatrixDifference::applyDifference(const glm::mat4& refMatrix, glm::mat4* matrix)
{
	glm::vec3 forward = Utils::getForward(*matrix);
	glm::rotate(forward, -angForwardByRight, Utils::getRight(refMatrix));
	glm::rotate(forward, -angForwardByUp, Utils::getUp(refMatrix));
	Utils::setForward(matrix, forward);

	glm::vec3 up = Utils::getUp(*matrix);
	glm::rotate(up, -angUpByForward, Utils::getForward(refMatrix));
	glm::rotate(up, -angUpByRight, Utils::getRight(refMatrix));
	Utils::setUp(matrix, up);

	glm::vec3 right = Utils::getRight(*matrix);
	glm::rotate(right, -angRightByForward, Utils::getForward(refMatrix));
	glm::rotate(right, -angRightByUp, Utils::getUp(refMatrix));
	Utils::setRight(matrix, right);
}
