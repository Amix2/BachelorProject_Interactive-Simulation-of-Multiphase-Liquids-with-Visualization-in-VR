#pragma once

#include <GL/glew.h>

namespace VR {
	class VRGLInterop {
	public:
		const unsigned int SCR_WIDTH = 1600;
		const unsigned int SCR_HEIGHT = 900;
		GLuint m_nResolveFramebufferIdLeft;
		GLuint m_nRenderFramebufferIdLeft;
		GLuint m_nDepthBufferIdLeft;
		GLuint m_nRenderTextureIdLeft;
		GLuint m_nResolveTextureIdLeft;

		GLuint m_nResolveFramebufferIdRight;
		GLuint m_nRenderFramebufferIdRight;
		GLuint m_nDepthBufferIdRight;
		GLuint m_nRenderTextureIdRight;
		GLuint m_nResolveTextureIdRight;

		bool activate();
		void phase1();
		void phase2();

	protected:
		//

	private:
		//
	};
}
