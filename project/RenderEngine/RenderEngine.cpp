#include "RenderEngine.hpp"

#include <gl/glew.h>
#include "Assets/Mesh_gl.hpp"
#include "Assets/AnimatedMesh_gl.hpp"
#include "Assets/Font_gl.hpp"
#include "Assets/ShaderObject_gl.hpp"
#include "Assets/FrameBuffer_gl.hpp"
#include "Assets/PixelBuffer_gl.hpp"
#include "Assets/Texture_gl.hpp"

#include "ReGlobal.hpp"

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
		// supress id 131154 PBO performance warning
		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION && id != 131154) {
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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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

		// opengl 4.5 extension
		if (glClipControl) {
			glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
		}

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CLIP_DISTANCE0);
		glEnable(GL_MULTISAMPLE);
		//glEnable(GL_FRAMEBUFFER_SRGB);

		//glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		clearColor = RGB(0, 0, 0);
	} else if (reci.renderEngineType == RenderEngineType::eRenderVulkan) {
		if (reci.createRenderWindow) {
			//throw "Not yet finished!";
			initWindowSystem();
#if SUPPORT_VULKAN_WINDOW
			vkWindow.init();
			vkWindow.showWindow(true);
#endif
		}
	}

	FT_Init_FreeType(&fontLibrary);
	counter = 1;
	lockdown = false;
}

void RenderEngine::release() {
	FT_Done_FreeType(fontLibrary);
	if (reci.createRenderWindow) {
		if (reci.renderEngineType == RenderEngineType::eRenderOpenGL)
			glWindow.deinit();
		else if (reci.renderEngineType == RenderEngineType::eRenderVulkan)
#if SUPPORT_VULKAN_WINDOW
			vkWindow.deinit();
#endif
		deinitWindowSystem();
	}
	delete this;
}

void RenderEngine::clearDebug() {
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
		processGlRequests();
		processGlFunctions();
		glDrawBuffer(GL_BACK);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}

void RenderEngine::clear() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}

void RenderEngine::bindDefaultFrameBuffer() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void RenderEngine::enableCulling(bool enable, bool backFace) {

	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glCullFace(backFace ? GL_BACK : GL_FRONT);
		GLenum cap = GL_CULL_FACE;
		if (enable) {
			glEnable(cap);
		} else {
			glDisable(cap);
		}
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

void RenderEngine::depthMask(bool mask) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glDepthMask(mask ? GL_TRUE : GL_FALSE);
	}
}

void RenderEngine::forceWriteDepth(bool forceWrite) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glDepthFunc(forceWrite ? GL_GREATER : GL_LESS);
	}
}

void RenderEngine::colorMask(bool r, bool g, bool b, bool a) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glColorMask(r, g, b, a);
	}
}

void RenderEngine::setScissorTest(bool enable) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		GLenum cap = GL_SCISSOR_TEST;
		if (enable) {
			glEnable(cap);
		} else {
			glDisable(cap);
		}
	}
}

void RenderEngine::setScissorRegion(int x, int y, int width, int height) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {

		int wx = 0;
		int wy = 0;
		glWindow.getWindowSize(wx, wy);

		glScissor(x, wy - (y + height), width, height);
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

void RenderEngine::stencilOp(StencilOp fail, StencilOp zfail, StencilOp zpass) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		int f = stencilOpToGlInt(fail);
		int zf = stencilOpToGlInt(zfail);
		int zp = stencilOpToGlInt(zpass);
		glStencilOp(f, zf, zp);
	}
}

void RenderEngine::stencilFunc(FuncConstants func, int ref, unsigned int mask) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glStencilFunc(funcConstantToGlInt(func), ref, mask);
	}
}

void RenderEngine::setClearColor(float r, float g, float b, float a) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glClearColor(r, g, b, a);
	}
}

void RenderEngine::activeTexture(int slot) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		glActiveTexture(GL_TEXTURE0 + slot);
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
		wglMakeCurrent(NULL, NULL);
		printf("Graphics reset!\n");
		printf("Locking down function calls");
		lockdown = true;
		return true;
	}
	return false;
}

void RenderEngine::updateViewPort(int width, int height) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		if (width != 0 && height != 0) {
			glViewport(0, 0, width, height);
			size.width = (float)width;
			size.height = (float)height;
		}
	}
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

IPixelBuffer* RenderEngine::createPixelBuffer() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return new PixelBuffer_gl();
	}
	return nullptr;
}

IReFont * RenderEngine::createFont() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return new Font_gl(fontLibrary);
	}
	return nullptr;
}

IWindow * RenderEngine::getMainWindow() {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		return &glWindow;
	} else if (reci.renderEngineType == RenderEngineType::eRenderVulkan) {
#if SUPPORT_VULKAN_WINDOW
		return &vkWindow;
#endif
	}
	return nullptr;
}

size_t RenderEngine::getMemoryUsage() const {
	return 0;
}

size_t RenderEngine::getMaxMemory() const {
	return 0;
}

void RenderEngine::toNormalizedDeviceSpace(float & x, float & y) {
	if (reci.renderEngineType == RenderEngineType::eRenderOpenGL) {
		float lx = (2.0f * x) / size.width - 1;
		float ly = (2.0f * y) / size.height - 1;

		x = lx;
		y = -ly;
	} else if (reci.renderEngineType == RenderEngineType::eRenderVulkan) {
	}
}

void RenderEngine::printInfo(GLinfo info) {
	printf("%s\nGPU: %s\nDriver:%s\nGLSL: %s\n", info.vendor, info.renderer, info.version, info.shadingLanguageVersion);
}

int RenderEngine::stencilOpToGlInt(StencilOp op) {
	int r = 0;
	switch (op) {
		case StencilOp::REPLACE:
			r = GL_REPLACE;
			break;
		case StencilOp::KEEP:
			r = GL_KEEP;
			break;
		default:
			break;
	}
	return r;
}

int RenderEngine::funcConstantToGlInt(FuncConstants funcConst) {
	int r = 0;
	switch (funcConst) {
		case FuncConstants::ALWAYS:
			r = GL_ALWAYS;
			break;
		case FuncConstants::LESS:
			r = GL_LESS;
			break;
		case FuncConstants::LEQUAL:
			r = GL_LEQUAL;
			break;
		case FuncConstants::EQUAL:
			r = GL_EQUAL;
			break;
		case FuncConstants::GEQUAL:
			r = GL_GEQUAL;
			break;
		case FuncConstants::GREATER:
			r = GL_GREATER;
			break;
		case FuncConstants::NOTEQUAL:
			r = GL_NOTEQUAL;
			break;
		case FuncConstants::NEVER:
			r = GL_NEVER;
			break;
		default:
			break;
	}
	return r;
}

IRenderEngine* CreateRenderEngine() {
	return new RenderEngine();
}