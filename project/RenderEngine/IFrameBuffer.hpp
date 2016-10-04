#ifndef IFRAMEBUFFER_HPP
#define IFRAMEBUFFER_HPP

class IFrameBuffer
{

public:

	virtual void init() = 0;
	virtual void release() = 0;

	virtual bool setupFrameBuffer(int width, int height, int nrColorAttachments, bool useDepth, bool multisample, bool useRenderBuffer) = 0;

	virtual void resolveToScreen(int bufferIndex = 0) = 0;

	virtual void bind() = 0;

	virtual void resize(int width, int height) = 0;
	virtual void setWindowSize(int windowWidth, int windowHeight) = 0;

	virtual bool isUsingRenderBuffer() const = 0;

};

#endif