#include "FrameBuffer.h"

void FrameBuffer::init()
{
	// create a texture object
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a renderbuffer object to store depth info
	GLuint rboId;
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	// create a framebuffer object
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER,			// 1. fbo target: GL_FRAMEBUFFER
		GL_COLOR_ATTACHMENT0,						// 2. attachment point
		GL_TEXTURE_2D,								// 3. tex target: GL_TEXTURE_2D
		textureId,									// 4. tex ID
		0											// 5. mipmap level: 0(base)
	);											

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,		// 1. fbo target: GL_FRAMEBUFFER
		GL_DEPTH_ATTACHMENT,						// 2. attachment point
		GL_RENDERBUFFER,						    // 3. rbo target: GL_RENDERBUFFER
		rboId										// 4. rbo ID
	);				

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,		// 1. fbo target: GL_FRAMEBUFFER
		GL_STENCIL_ATTACHMENT,						// 2. attachment point
		GL_RENDERBUFFER,						    // 3. rbo target: GL_RENDERBUFFER
		rboId										// 4. rbo ID
	);

// check FBO status
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		LOG_F(ERROR, std::string("FBO: " + status).c_str());
}
