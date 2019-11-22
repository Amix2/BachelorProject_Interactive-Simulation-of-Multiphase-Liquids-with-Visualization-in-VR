#pragma once
#include <inputDispatcher/listener/KeyInputListener.h>
#include <digitalHand/HandDataProvider.h>
#include <materialObjects/MaterialObject.h>
#include <materialObjects/PyramidPointerMaterialObject.h>
#include <selectableObject/SelectableObjectManager.h>
#include <memory>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <Utils.h>

class PyramidPointerMaterialObject;

enum Hand {
	RIGHT_HAND,
	LEFT_HAND
};

struct MatrixDifference {
	float angForwardByRight;
	float angForwardByUp;
	float angUpByRight;
	float angUpByForward;
	float angRightByUp;
	float angRightByForward;

	void createDifference(const glm::mat4& refMatrix, const glm::mat4& otherMatrix);
	void applyDifference(const glm::mat4& refMatrix, glm::mat4* matrix);
};

// aka VR Controller
class DigitalHand : public KeyInputListener, public MaterialObject
{
private:
	const inline static float PROXY_GRAB_DISTANCE = 10;
	const inline static float ANGLE_GRAB_DIFFERENCE = 0.55f;	// ~30 deg
	const inline static glm::mat4 SCALE_MATRIX = glm::scale(glm::mat4{ 1.0f }, { 100, 100, 100 });
	VR::VRGLInterop* vrglinterop;
	HandDataProvider* m_dataProvider;
	std::unique_ptr<PyramidPointerMaterialObject> m_pyramid;
	Hand m_hand;
	ShaderProgram m_handShader;
	SelectableObject* m_selectedObject = nullptr;
	glm::mat4 m_handMatrix;
	glm::mat4 m_grabMatrixOffset;
	MatrixDifference m_matrixDifference;

	glm::mat4 getMyHandMatrix() const;
	bool tryGrabDistance();
	bool tryGrabAngle();
	void teleportObjectToHand(SelectableObject* obj);
	void setGrabMatrixOffset();
public:

	DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader);
	DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader, VR::VRGLInterop* vrglinterop);

	void init();
	void load(const glm::mat4& view, const glm::mat4& projection) const;

	const glm::mat4 getPyramidModel() const;

	void handleKeyPress(int key, KeyState state, float deltaTime) override;

	void update();
};

