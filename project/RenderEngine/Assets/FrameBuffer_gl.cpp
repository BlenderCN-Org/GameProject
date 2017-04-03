#include "FrameBuffer_gl.hpp"
#include "../Utils/MemoryManager.hpp"

bool FrameBuffer_gl::init(FrameBufferCreateInfo *createInfo) {
	glGenFramebuffers(1, &framebuffer);

	bool success = true;

	width = windowWidth = createInfo->width;
	height = windowHeight = createInfo->height;

	depth = createInfo->useDepth;
	stencil = createInfo->useStencil;
	colorAttachmentCount = createInfo->nrColorBuffers;
	multiSample = createInfo->useMultisample;
	multiSampleCount = createInfo->mutlisample;
	usingRenderBuffers = createInfo->useRenderbuffer;

	success = setupFrameBuffer();

	return success;
}

void FrameBuffer_gl::release() {
	if ( created ) {
		for ( int i = 0; i < colorAttachmentCount; i++ ) {
			if ( usingRenderBuffers ) {
				glDeleteRenderbuffers(1, &colorAttachments[i]);
			} else {
				glDeleteTextures(1, &colorAttachments[i]);
			}
		}
		if ( depth ) {
			if ( usingRenderBuffers ) {
				glDeleteRenderbuffers(1, &depthAttachment);
			} else {
				glDeleteTextures(1, &depthAttachment);
			}
		}

		delete colorAttachments;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &framebuffer);

	delete this;
}

bool FrameBuffer_gl::setupFrameBuffer() {
	bool success = true;

	//useRenderBuffer = false; // FORCE OFF DUE TO RESOLVE ISSUES

	// for now to not make errors when deleting
	//useRenderBuffer = false;

	colorAttachments = new GLuint[colorAttachmentCount];
	bind();

	GLenum* drawBuffers = new GLenum[colorAttachmentCount];

	if ( usingRenderBuffers ) {
		if ( depth )
			depthAttachment = createDepthRenderBuffer(width, height, stencil);
		for ( int i = 0; i < colorAttachmentCount; i++ ) {
			colorAttachments[i] = createColorRenderBuffer(width, height, GL_RGBA, GL_COLOR_ATTACHMENT0 + i);
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
	} else {
		if ( depth )
			depthAttachment = createDepthTexture(width, height, stencil);
		for ( int i = 0; i < colorAttachmentCount; i++ ) {
			colorAttachments[i] = createColorTexture(width, height, GL_RGBA, GL_COLOR_ATTACHMENT0 + i);
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
	}

	glDrawBuffers(colorAttachmentCount, drawBuffers);

	delete [] drawBuffers;

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if ( status != GL_FRAMEBUFFER_COMPLETE ) {
		char* msg = "Failed to create framebuffer";

		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, status, GL_DEBUG_SEVERITY_HIGH, sizeof(msg), msg);

		success = false;

		for ( int i = 0; i < colorAttachmentCount; i++ ) {
			if ( usingRenderBuffers ) {
				glDeleteRenderbuffers(1, &colorAttachments[i]);
			} else {
				glDeleteTextures(1, &colorAttachments[i]);
			}
		}
		if ( depth ) {
			if ( usingRenderBuffers ) {
				glDeleteRenderbuffers(1, &depthAttachment);
			} else {
				glDeleteTextures(1, &depthAttachment);
			}
		}
		delete [] colorAttachments;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	created = success;

	//glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glClearStencil(0x00);
	glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
	glStencilMask(0xFF);

	return success;
}

void FrameBuffer_gl::resize(int _width, int _height) {
	bind();

	width = _width;
	height = _height;

	if ( usingRenderBuffers ) {
		if ( depth ) {
			glBindRenderbuffer(GL_RENDERBUFFER, depthAttachment);
			if ( stencil ) {
				if ( multiSample )
					glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSampleCount, GL_DEPTH24_STENCIL8, width, height);
				else
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthAttachment);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthAttachment);
			} else {
				if ( multiSample )
					glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, width, height);
				else
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthAttachment);
			}
		}

		for ( int i = 0; i < colorAttachmentCount; i++ ) {
			glBindRenderbuffer(GL_RENDERBUFFER, colorAttachments[i]);

			if ( multiSample )
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSampleCount, GL_RGBA8, width, height);
			else
				glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, colorAttachments[i]);
		}
	} else {
		if ( depth ) {
			glBindTexture(GL_TEXTURE_2D, depthAttachment);
			if ( stencil ) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
			} else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
			}
		}

		for ( int i = 0; i < colorAttachmentCount; i++ ) {
			glBindTexture(GL_TEXTURE_2D, colorAttachments[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i], 0);
		}
	}
}

void FrameBuffer_gl::setWindowSize(int _windowWidth, int _windowHeight) {
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;
}

void FrameBuffer_gl::resolveToScreen(int bufferIndex) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	if ( bufferIndex < colorAttachmentCount ) {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + bufferIndex);
	} else {
		glReadBuffer(GL_COLOR_ATTACHMENT0);
	}
	//glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, width, height, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer_gl::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

bool FrameBuffer_gl::isUsingRenderBuffer() const {
	return usingRenderBuffers;
}

GLuint FrameBuffer_gl::createDepthTexture(int width, int height, bool stencil) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if ( stencil )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	if ( stencil ) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	} else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

	return texture;
}

GLuint FrameBuffer_gl::createDepthRenderBuffer(int width, int height, bool stencil) {
	GLuint renderBuffer;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	if ( stencil ) {
		if ( multiSample )
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSampleCount, GL_DEPTH24_STENCIL8, width, height);
		else
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
	} else {
		if ( multiSample )
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSampleCount, GL_DEPTH_COMPONENT24, width, height);
		else
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
	}

	return renderBuffer;
}

GLuint FrameBuffer_gl::createColorTexture(int width, int height, int format, int attachment) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

	return texture;
}

GLuint FrameBuffer_gl::createColorRenderBuffer(int width, int height, int format, int attachment) {
	GLuint renderBuffer;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

	if ( multiSample )
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, format, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer);

	return renderBuffer;
}