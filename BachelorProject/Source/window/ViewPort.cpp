#include "ViewPort.h"
#include <iostream>

ViewPort::ViewPort(Window& window, double relativeX, double relativeY, double relativeWidth, double relativeHeight)
	: window{ &window }
	, frameBufferWidth{ window.getWidth() }
	, frameBufferHeight{ window.getHeight() }
	, relativeX{ relativeX }
	, relativeY{ relativeY }
	, relativeWidth{ relativeWidth }
	, relativeHeight{ relativeHeight }
{
	window.subscribeForWindowSizeChanges(this);
}

ViewPort::~ViewPort()
{
	window->unsubscribeWindowSizeListener(this);
}

void ViewPort::handleWindowResize(int width, int height)
{
	frameBufferWidth = width;
	frameBufferHeight = height;
}
