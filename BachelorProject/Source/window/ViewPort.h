#pragma once

#include "./listener/WindowSizeListener.h"
#include "Window.h"
#include "FrameBuffer.h"

class ViewPort : public FrameBufferSizeListener
{
public:
	ViewPort(Window& window, double relativeX, double relativeY, double relativeWidth, double relativeHeight);
	ViewPort(FrameBuffer& window, double relativeX, double relativeY, double relativeWidth, double relativeHeight);


	void handleFrameBufferResize(int width, int height) override;
	int getX() const { return frameBufferWidth * relativeX; }
	int getY() const { return frameBufferHeight * relativeY; }
	int getWidth() const { return frameBufferWidth * relativeWidth; }
	int getHeight() const { return frameBufferHeight * relativeHeight; }
private:
	unsigned int frameBufferWidth;
	unsigned int frameBufferHeight;

	double relativeX;
	double relativeY;
	double relativeWidth;
	double relativeHeight;
};

