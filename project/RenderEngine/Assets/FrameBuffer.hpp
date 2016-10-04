#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "../IFrameBuffer.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

class FrameBuffer : public IFrameBuffer
{

public:

	FrameBuffer() : created(false) {};

	virtual void init();
	virtual void release();

	virtual bool setupFrameBuffer(int width, int height, int nrColorAttachments, bool useDepth, bool multisample, bool useRenderBuffer);

	virtual void resize(int width, int height);
	virtual void setWindowSize(int windowWidth, int windowHeight);

	virtual void resolveToScreen(int bufferIndex = 0);

	virtual void bind();

	virtual bool isUsingRenderBuffer() const;

private:
	
	GLuint createDepthTexture(int width, int height);
	GLuint createDepthRenderBuffer(int width, int height);

	GLuint createColorTexture(int width, int height, int format, int attachment);
	GLuint createColorRenderBuffer(int width, int height, int format, int attachment);

	int width;
	int height;

	int windowWidth;
	int windowHeight;

	bool created;

	bool depth;
	bool multiSample;
	bool usingRenderBuffers;

	GLuint framebuffer;
	
	int colorAttachmentCount;

	GLuint* colorAttachments;
	GLuint depthAttachment;


};

#endif