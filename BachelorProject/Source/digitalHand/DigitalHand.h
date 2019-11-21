#pragma once
#include <inputDispatcher/listener/KeyInputListener.h>
#include <digitalHand/HandDataProvider.h>
#include <materialObjects/MaterialObject.h>
#include <materialObjects/PyramidPointerMaterialObject.h>
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
public:

	DigitalHand(HandDataProvider* dataprovider, Hand hand);

	void init();
	void load(const glm::mat4& view, const glm::mat4& projection) const;

	void handleKeyPress(int key, KeyState state, float deltaTime) override;

	void update();
};

