#ifndef IFRAMEBUFFER_HPP
#define IFRAMEBUFFER_HPP

struct FrameBufferCreateInfo
{
	int width;
	int height;
	int nrColorBuffers;
	bool useDepth;
	bool useMultisample;
	char mutlisample;
	bool useRenderbuffer;
	bool useStencil;
};

class IFrameBuffer
{

public:

	virtual bool init(FrameBufferCreateInfo *createInfo) = 0;
	virtual void release() = 0;

	//virtual bool setupFrameBuffer(int width, int height, int nrColorAttachments, bool useDepth, bool multisample, bool useRenderBuffer, bool stencil) = 0;

	virtual void resolveToScreen(int bufferIndex = 0) = 0;

	virtual void bind() = 0;

	virtual void resize(int width, int height) = 0;
	virtual void setWindowSize(int windowWidth, int windowHeight) = 0;

	virtual bool isUsingRenderBuffer() const = 0;

};

#endif