#pragma once
#include <inputDispatcher/listener/KeyInputListener.h>
#include <digitalHand/HandDataProvider.h>
#include <materialObjects/MaterialObject.h>
#include <materialObjects/MaterialObjectOwner.h>
#include <materialObjects/PyramidPointerMaterialObject.h>
#include <selectableObject/SelectableObjectManager.h>
#include <memory>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <Utils.h>
#include <actionControllers/VRActionController.h>
#include <VR/SteamIVRInput.h>
#include <VR/VRInterface.h>

class PyramidPointerMaterialObject;

enum Hand {
	RIGHT_HAND,
	LEFT_HAND
};

// aka VR Controller
class DigitalHand : public MaterialObjectOwner, public MaterialObject
{
private:
	const inline static float PROXY_GRAB_DISTANCE = 10;
	const inline static float ANGLE_GRAB_DIFFERENCE = 0.55f;	// ~30 deg
	const inline static glm::mat4 SCALE_MATRIX = glm::scale(glm::mat4{ 1.0f }, { 100, 100, 100 });
	VR::VRInterface* m_vrInterface;
	SteamIVRInput* m_steamInput;
	HandDataProvider* m_dataProvider;
	std::unique_ptr<PyramidPointerMaterialObject> m_pyramid;
	Hand m_hand;
	ShaderProgram m_handShader;
	SelectableObject* m_selectedObject = nullptr;
	VRActionController* m_selectedActionController = nullptr;
	glm::mat4 m_handMatrix;
	glm::mat4 m_grabMatrixOffset = glm::mat4(1);
	int m_deviceIndex = -1;

	glm::mat4 getMyHandMatrix() const;
	bool tryGrabDistance();
	bool tryGrabAngle();
	void grab(SelectableObject* object);
	void release();
	void teleportObjectToHand();
	void moveObjectWithHand();
	void setGrabMatrixOffset(SelectableObject* object);
public:

	DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader);
	DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader, VR::VRInterface* vrInterface, SteamIVRInput* steamInput);

	void init();
	void load(const glm::mat4& view, const glm::mat4& projection) const;

	glm::mat4 getModel() const override;
	void update();
};

