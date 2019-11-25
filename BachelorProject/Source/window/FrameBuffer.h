#pragma once

#include <GL/glew.h>
#include <Logger.h>
#include <string>

class FrameBuffer {
public:
	FrameBuffer(unsigned int width, unsigned int height) : width{ width }, height{ height } {};
	void init();
	unsigned int getWidth() const { return width; }
	unsigned int getHeight() const { return height; }
	void drawTo() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID); }
	void readFrom() const { glBindFramebuffer(GL_READ_FRAMEBUFFER, ID); }

private:
	GLuint ID;
	GLuint rboID;
	unsigned int width;
	unsigned int height;
};