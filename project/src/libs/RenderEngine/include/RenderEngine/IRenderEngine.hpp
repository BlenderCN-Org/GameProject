#ifndef IRENDERENGINE_HPP
#define IRENDERENGINE_HPP

#if _WIN32

#ifdef renderEngine_lib_EXPORTS
#define RENDERENGINE_API _declspec(dllexport)
#else
#define RENDERENGINE_API _declspec(dllimport)
#endif

#elif defined(__GNUC__)

#ifdef renderEngine_lib_EXPORTS
#define RENDERENGINE_API __attribute__((visibility("default")))
#else
#define RENDERENGINE_API 
#endif

#else

#define RENDERENGINE_API 
#pragma warning Unknown dynamic link import/export semantics.

#endif

#include "IAnimatedMesh.hpp"
#include "IShaderObject.hpp"
#include "IFrameBuffer.hpp"
#include "IPixelBuffer.hpp"
#include "ITexture.hpp"

#include "IFont.hpp"

#include "IWindow.hpp"

enum class RenderEngineType {
	eRenderNone = -1,
	eRenderOpenGL = 0,
	eRenderVulkan = 1,
};

enum class SType {
	sOpenGLWindowCreateInfo = 0,
	sRenderEngineCreateInfo = 1,
	sVulkanWindowCreateInfo = 2,
};

struct OpenGLWindowCreateInfo {
	SType stype;
	int majorVersion;
	int minorVersion;
};

struct VulkanWindowCreateInfo {
	SType stype;
	int presentQueueCount; // number of present queues
	int commandQueueCount; // number of command queues
	int transfereQueueCount; // number of transfer queues

	int preferredGPU; // gpu selection -1 to let application decide
};

typedef void* (*PFN_EngineAllocator(size_t));
typedef void(*PFN_EngineDeallocator(void*));

struct AllocatorInfo {
	PFN_EngineAllocator pfnEngineAlloc;
	PFN_EngineDeallocator pfnEngineDealloc;
};

struct RenderEngineCreateInfo {
	SType stype;
	bool createRenderWindow;
	RenderEngineType renderEngineType;
	//AllocatorInfo globalAllocators;
	void* pNext;
};

enum class FuncConstants : int {
	ALWAYS,
	LESS,
	LEQUAL,
	EQUAL,
	GEQUAL,
	GREATER,
	NOTEQUAL,
	NEVER
};

enum class StencilOp :int {
	REPLACE,
	KEEP,
};

struct ScissorInfo {
	int enableCtr;
	int x, y;
	int w, h;
};

class IRenderEngine {
public:

	virtual void init(RenderEngineCreateInfo &createInfo) = 0;
	virtual void release() = 0;
	virtual void clearDebug() = 0;
	virtual void clear() = 0;

	virtual void enableCulling(bool enable, bool backFace = true) = 0;

	virtual void bindDefaultFrameBuffer() = 0;

	virtual void setDepthTest(bool enable) = 0;
	virtual void depthMask(bool mask) = 0;
	virtual void forceWriteDepth(bool forceWrite) = 0;

	virtual void colorMask(bool r, bool g, bool b, bool a) = 0;

	virtual const ScissorInfo pushScissorRegion(int x, int y, int width, int height) = 0;
	virtual void popScissorRegion(const ScissorInfo prevScissor) = 0;

	virtual void clearStencil() = 0;

	virtual void setStencilTest(bool enable) = 0;
	virtual void stencilMask(unsigned int mask) = 0;
	virtual void stencilClear(int mask) = 0;
	virtual void stencilOp(StencilOp fail, StencilOp zfail, StencilOp zpass) = 0;
	virtual void stencilFunc(FuncConstants func, int ref, unsigned int mask) = 0;
	virtual void setClearColor(float r, float g, float b, float a) = 0;

	virtual void activeTexture(int slot) = 0;

	virtual void setBlending(bool enable) = 0;

	virtual bool getGraphicsReset() const = 0;

	virtual void updateViewPort(int width, int height) = 0;

	virtual IMesh* createMesh() = 0;
	virtual IAnimatedMesh* createAnimatedMesh() = 0;
	//virtual IRenderObject* createRenderObject() = 0;
	virtual IShaderObject* createShaderObject() = 0;
	virtual ITexture* createTexture() = 0;

	virtual IFrameBuffer* createFrameBuffer() = 0;

	virtual IPixelBuffer* createPixelBuffer() = 0;

	virtual IReFont* createFont() = 0;

	virtual IWindow* getMainWindow() = 0;

	virtual size_t getMemoryUsage() const = 0;
	virtual size_t getMaxMemory() const = 0;

	virtual void toNormalizedDeviceSpace(float &x, float &y) = 0;
};

extern "C" RENDERENGINE_API IRenderEngine* CreateRenderEngine();

typedef IRenderEngine* (*CreateRenderEngineProc)();

#endif