#pragma once
#include <inputDispatcher/listener/KeyInputListener.h>
#include <digitalHand/DataProvider.h>
#include <materialObjects/MaterialObject.h>
#include <materialObjects/PyramidPointerMaterialObject.h>
#include <memory>

enum class Hand {
	RIGHT,
	LEFT
};

// aka VR Controller
class DigitalHand : public KeyInputListener, public MaterialObject
{
private:
	DataProvider* dataProvider;
	std::shared_ptr<PyramidPointerMaterialObject> pyramid;
public:

	DigitalHand(DataProvider* dataprovider);

	void init();
	void load(const glm::mat4& view, const glm::mat4& projection) const;

	void handleKeyPress(int key, KeyState state, float deltaTime) override;

	void update();
};

