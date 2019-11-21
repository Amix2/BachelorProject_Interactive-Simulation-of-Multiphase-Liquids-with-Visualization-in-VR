#pragma once
#include <inputDispatcher/listener/KeyInputListener.h>
#include <digitalHand/HandDataProvider.h>
#include <materialObjects/MaterialObject.h>
#include <materialObjects/PyramidPointerMaterialObject.h>
#include <selectableObject/SelectableObjectManager.h>
#include <memory>

enum Hand {
	RIGHT_HAND,
	LEFT_HAND
};

// aka VR Controller
class DigitalHand : public KeyInputListener, public MaterialObject
{
private:
	HandDataProvider* m_dataProvider;
	std::shared_ptr<PyramidPointerMaterialObject> m_pyramid;
	Hand m_hand;
	ShaderProgram m_handShader;
	SelectableObject* m_selectedObject = nullptr;

	glm::mat4 getMyHandMatrix();
public:

	DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader);
	DigitalHand(HandDataProvider* dataprovider, Hand hand, ShaderProgram handShader, const VR::VRGLInterop& vrglinterop);

	void init();
	void load(const glm::mat4& view, const glm::mat4& projection) const;

	void handleKeyPress(int key, KeyState state, float deltaTime) override;

	void update();
};

