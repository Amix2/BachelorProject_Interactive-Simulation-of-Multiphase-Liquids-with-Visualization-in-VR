#pragma once
class FrameBufferSizeListener
{
public:
	void virtual handleFrameBufferResize(int width, int height) = 0;
};

