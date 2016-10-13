#ifndef RENDERENGINE_HPP
#define RENDERENGINE_HPP

#include "IRenderEngine.hpp"
#include "DataTypes\Colors.hpp"
#include "ShaderState.hpp"
#include "Assets\RenderObject.hpp"
#include "Camera.hpp"

#include <Allocators\FrameAllocator.hpp>
#include <Allocators\PoolAllocator.hpp>

#include <freetype\ft2build.h>
#include FT_FREETYPE_H


#include <vld.h>

class RenderEngine : public IRenderEngine
{
public:

	RenderEngine() {};

	virtual void init();
	virtual void release();
	virtual void renderDebugFrame();

	virtual void setDepthTest(bool enable);

	virtual void setStencilTest(bool enable);

	virtual bool getGraphicsReset() const;

	virtual void updateViewPort(int width, int height);

	virtual ICamera* createCamera();
	virtual void setActiveCamera(ICamera * camera);
	//virtual void* getViewMatrixPtr();

	virtual IMesh* createMesh();
	virtual IAnimatedMesh* createAnimatedMesh();
	//virtual IRenderObject* createRenderObject();
	virtual IShaderObject* createShaderObject();
	virtual ITexture* createTexture();

	virtual IFrameBuffer* createFrameBuffer();

	virtual IText* createText();

	virtual IFont* createFont();

	virtual bool isRenderObjectIsInFrustum(IRenderObject* renderObject);
	
	virtual size_t getMemoryUsage() const;
	virtual size_t getMaxMemory() const;

private:

	int counter = 0;
	
	RGB clearColor;
	ShaderState shaderState;
	Camera cam;

	PoolAllocator<RenderObject> objectPool;
	//FrameAllocator frameAlloc;

	FT_Library fontLibrary;

};

#endif