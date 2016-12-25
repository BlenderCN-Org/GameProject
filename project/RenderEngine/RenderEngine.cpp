#include "RenderEngine.hpp"

#include <gl\glew.h>
#include "Assets\Mesh_gl.hpp"
#include "Assets\AnimatedMesh_gl.hpp"
#include "Assets\Font_gl.hpp"
#include "Assets\ShaderObject_gl.hpp"
#include "Assets\FrameBuffer_gl.hpp"
#include "Assets\Texture_gl.hpp"
#include "Camera.hpp"


#include "Shader.h"

#include "Utils\MemoryManager.hpp"
#include "Utils\FrameAllocator_static.hpp"

#ifdef _DEBUG
extern "C"
{
	void __stdcall openglCallbackFunction(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		void* userParam) {
		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
			printf("---------------------opengl-callback-start------------\n");
			printf("message: %s\n", message);
			printf("type: ");
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				printf("ERROR");
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				printf("DEPRECATED_BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				printf("UNDEFINED_BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				printf("PORTABILITY");
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				printf("PERFORMANCE");
				break;
			case GL_DEBUG_TYPE_OTHER:
				printf("OTHER");
				break;
			}
			printf("\n");

			printf("id: %d\n", id);
			printf("severity: ");
			switch (severity) {
			case GL_DEBUG_SEVERITY_LOW:
				printf("LOW");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				printf("MEDIUM");
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				printf("HIGH");
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				printf("NOTIFICATION");
				break;
			}
			printf("\n");
			printf("---------------------opengl-callback-end--------------\n");
		}
	}
}
#endif

void RenderEngine::init(RenderEngineCreateInfo &createInfo) {
	reci = createInfo;
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		if (reci.createRenderWindow) {
			//throw "Not yet finished!";
			initWindowSystem();
			glWindow.init();

			glWindow.showWindow(true);
		}

		glewInit();

#ifdef _DEBUG
		if (glDebugMessageCallback) {
			printf("Register OpenGL debug callback\n");
			glDebugMessageCallback((GLDEBUGPROC)openglCallbackFunction, nullptr);
			GLuint unusedIds = 0;
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
		} else
			printf("glDebugMessageCallback not available\n");
#endif

		info.vendor = (const char*)glGetString(GL_VENDOR);
		info.renderer = (const char*)glGetString(GL_RENDERER);
		info.version = (const char*)glGetString(GL_VERSION);
		info.shadingLanguageVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

		printInfo(info);

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		clearColor = RGB(0, 0, 0);
	}
	else if (reci.renderEngineType == RenderEngineType::eRenderVulkan)
	{
		if (reci.createRenderWindow) {
			//throw "Not yet finished!";
			initWindowSystem();
			vkWindow.init();

			vkWindow.showWindow(true);
		}
	}
	//objectPool = PoolAllocator<RenderObject>(10);

	MemoryManager* mgr = MemoryManager::getMemoryManager();

	//frameAlloc = FrameAllocator(1024, mgr);
	FrameAllocator* frameAllocator = FrameAllocator_static::getFrameAllocator();

	int* i = frameAllocator->allocate<int>(1);
	i[0] = 5;
	i[1] = 10;

	//cam.perspective = glm::perspectiveFov(glm::radians(75.0f), 1280.0f, 720.0f, 0.0001f, 100.0f);

	FT_Init_FreeType(&fontLibrary);
	counter = 1;

}

void RenderEngine::release() {
	FT_Done_FreeType(fontLibrary);
	if (reci.createRenderWindow) {
		glWindow.deinit();
		deinitWindowSystem();
	}
	delete this;
	// static class so this is possible
	FrameAllocator_static::release();
	MemoryManager::release();
}

void RenderEngine::renderDebugFrame() {

	FrameAllocator* frameAllocator = FrameAllocator_static::getFrameAllocator();

	frameAllocator->reset();

	counter += 20;

	if (counter > 32 * KB) {
		counter = 1;
	}

	HSV c = RGB2HSV(clearColor);
	c.h += 1.0f;
	c.v = 0.5f;
	c.s = 1.0f;
	clearColor = HSV2RGB(c);
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		//glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

}

void RenderEngine::setDepthTest(bool enable) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {

		GLenum cap = GL_DEPTH_TEST;
		if (enable) {
			glEnable(cap);
		} else {
			glDisable(cap);
		}
	}
}

void RenderEngine::clearStencil() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {

		glClear(GL_STENCIL_BUFFER_BIT);
	}
}

void RenderEngine::setStencilTest(bool enable) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		GLenum cap = GL_STENCIL_TEST;

		if (enable) {
			glEnable(cap);
		} else {
			glDisable(cap);
		}
	}
}

void RenderEngine::stencilMask(unsigned int mask) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glStencilMask(mask);
	}
}

void RenderEngine::stencilClear(int mask) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glClearStencil(mask);
	}
}

void RenderEngine::stencilOp(unsigned int fail, unsigned int zfail, unsigned int zpass) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glStencilOp(fail, zfail, zpass);
	}
}

void RenderEngine::stencilFunc(unsigned int func, int ref, unsigned int mask) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glStencilFunc(func, ref, mask);
	}
}

void RenderEngine::setBlending(bool enable) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		GLenum cap = GL_BLEND;

		if (enable) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			glEnable(cap);
		} else {
			glDisable(cap);
		}
	}
}

bool RenderEngine::getGraphicsReset() const {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		GLenum status = glGetGraphicsResetStatus();
		if (status == GL_NO_ERROR)
			return false;
		while (glGetGraphicsResetStatus() != GL_NO_ERROR);

		return true;
	}
	return false;
}

void RenderEngine::updateViewPort(int width, int height) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		if (width != 0 && height != 0) {
			glViewport(0, 0, width, height);
		}
	}
}

ICamera * RenderEngine::createCamera() {
	return new Camera();
}

void RenderEngine::setActiveCamera(ICamera * camera) {
	shaderState.setActiveCamera(camera);
}

IMesh * RenderEngine::createMesh() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return new Mesh_gl();
	}
	return nullptr;
}

IAnimatedMesh * RenderEngine::createAnimatedMesh() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return new AnimatedMesh_gl();
	}
	return nullptr;
}

//IRenderObject * RenderEngine::createRenderObject() {
//	return new RenderObject();
//}

IShaderObject * RenderEngine::createShaderObject() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return new ShaderObject_gl();
	}
	return nullptr;
}

ITexture * RenderEngine::createTexture() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return new Texture_gl();
	}
	return nullptr;
}

IFrameBuffer * RenderEngine::createFrameBuffer() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return new FrameBuffer_gl();
	}
	return nullptr;
}

IFont * RenderEngine::createFont() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return new Font_gl(fontLibrary);
	}
	return nullptr;
}

IWindow * RenderEngine::getMainWindow() {
	return &glWindow;
}

size_t RenderEngine::getMemoryUsage() const {
	return MemoryManager::getMemoryManager()->getUsedHeap();
}

size_t RenderEngine::getMaxMemory() const {
	return MemoryManager::getMemoryManager()->getHeapSize();
}

void RenderEngine::printInfo(GLinfo info) {
	printf("%s\nGPU: %s\nDriver:%s\nGLSL: %s\n", info.vendor, info.renderer, info.version, info.shadingLanguageVersion);
}

IRenderEngine* CreateRenderEngine() {
	return new RenderEngine();
}
