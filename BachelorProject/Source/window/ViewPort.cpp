#include "ViewPort.h"
#include <iostream>

ViewPort::ViewPort(Window& window, double relativeX, double relativeY, double relativeWidth, double relativeHeight)
	: frameBufferWidth{ window.getWidth() }
	, frameBufferHeight{ window.getHeight() }
	, relativeX{ relativeX }
	, relativeY{ relativeY }
	, relativeWidth{ relativeWidth }
	, relativeHeight{ relativeHeight }
{
	window.subscribeForFrameBufferSizeChanges(this);
}

ViewPort::ViewPort(FrameBuffer& framebuffer, double relativeX, double relativeY, double relativeWidth, double relativeHeight)
	: frameBufferWidth{ framebuffer.getWidth() }
	, frameBufferHeight{ framebuffer.getHeight() }
	, relativeX{ relativeX }
	, relativeY{ relativeY }
	, relativeWidth{ relativeWidth }
	, relativeHeight{ relativeHeight }
{}


void ViewPort::handleFrameBufferResize(int width, int height)
{
	frameBufferWidth = width;
	frameBufferHeight = height;
}
