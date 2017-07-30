#ifndef RENDERENGINE_HPP
#define RENDERENGINE_HPP

#include "Window\Window.hpp"
#include "Window\WindowSystem.hpp"

#undef RGB

#include "IRenderEngine.hpp"
#include "DataTypes\Colors.hpp"

#include <Allocators\FrameAllocator.hpp>
#include <Allocators\PoolAllocator.hpp>

#include <freetype\ft2build.h>
#include FT_FREETYPE_H

//#include <vld.h>

struct GLinfo {
	const char* vendor;
	const char* renderer;
	const char* version;
	const char* shadingLanguageVersion;
};

struct VKinfo {
	const char* vendor;
	const char* renderer;
	const char* version;
};

struct DisplaySize {
	float width;
	float height;
};

class RenderEngine : public IRenderEngine {
public:

	RenderEngine() {};

	virtual void init(RenderEngineCreateInfo &createInfo);
	virtual void release();
	virtual void renderDebugFrame();

	virtual void setDepthTest(bool enable);

	virtual void clearStencil();

	virtual void setStencilTest(bool enable);
	virtual void stencilMask(unsigned int mask);
	virtual void stencilClear(int mask);
	virtual void stencilOp(unsigned int fail, unsigned int zfail, unsigned int zpass);
	virtual void stencilFunc(unsigned int func, int ref, unsigned int mask);

	virtual void setBlending(bool enable);

	virtual bool getGraphicsReset() const;

	virtual void updateViewPort(int width, int height);

	virtual IMesh* createMesh();
	virtual IAnimatedMesh* createAnimatedMesh();
	virtual IShaderObject* createShaderObject();
	virtual ITexture* createTexture();

	virtual IFrameBuffer* createFrameBuffer();

	virtual IPixelBuffer* createPixelBuffer();

	virtual IReFont* createFont();

	virtual IWindow* getMainWindow();

	virtual size_t getMemoryUsage() const;
	virtual size_t getMaxMemory() const;

	virtual void toNormalizedDeviceSpace(float &x, float &y);

private:

	void printInfo(GLinfo info);

	int counter = 0;

	GLinfo info;

	RGB clearColor;
	FT_Library fontLibrary;

	GLWindow glWindow;

	DisplaySize size;

#if SUPPORT_VULKAN_WINDOW
	VKWindow vkWindow;
#endif

	RenderEngineCreateInfo reci;
};

#endif