#pragma once

#include "./listener/WindowSizeListener.h"
#include "Window.h"

class ViewPort : public WindowSizeListener
{
public:
	ViewPort(Window& window, double relativeX, double relativeY, double relativeWidth, double relativeHeight);
	~ViewPort();

	void handleWindowResize(int width, int height) override;
	int getX() const { return frameBufferWidth * relativeX; }
	int getY() const { return frameBufferHeight * relativeY; }
	int getWidth() const { return frameBufferWidth * relativeWidth; }
	int getHeight() const { return frameBufferHeight * relativeHeight; }
private:
	int frameBufferWidth;
	int frameBufferHeight;

	double relativeX;
	double relativeY;
	double relativeWidth;
	double relativeHeight;

	Window* window;
};

