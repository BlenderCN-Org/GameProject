#ifndef FRAMEBUFFER_GL_HPP
#define FRAMEBUFFER_GL_HPP

#include "../IFrameBuffer.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

class FrameBuffer_gl : public IFrameBuffer {
public:

	FrameBuffer_gl() : created(false) {};

	virtual bool init(FrameBufferCreateInfo *createInfo);
	virtual void release();

	virtual void clear();

	virtual void resize(int width, int height);
	virtual void setWindowSize(int windowWidth, int windowHeight);

	virtual void resolveToScreen(int bufferIndex = 0);

	virtual void bind();

	virtual bool isUsingRenderBuffer() const;

private:

	bool setupFrameBuffer();

	GLuint createDepthTexture(int width, int height, bool stencil);
	GLuint createDepthRenderBuffer(int width, int height, bool stencil);

	GLuint createColorTexture(int width, int height, int format, int attachment);
	GLuint createColorRenderBuffer(int width, int height, int format, int attachment);

	int width;
	int height;

	int windowWidth;
	int windowHeight;

	bool created;

	bool stencil;
	bool depth;
	bool multiSample;
	char multiSampleCount;
	bool usingRenderBuffers;

	GLuint framebuffer;

	int colorAttachmentCount;

	GLuint* colorAttachments;
	GLuint depthAttachment;
};

#endif