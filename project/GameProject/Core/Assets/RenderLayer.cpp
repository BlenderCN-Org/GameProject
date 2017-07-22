#include "RenderLayer.hpp"

void RenderLayer::init(IRenderEngine* re) {
	defaultLayer = false;
	renderBuffer = re->createFrameBuffer();
	renderLayerShader = re->createShaderObject();

	FrameBufferCreateInfo fbci{0};

	fbci.height = 100;
	fbci.width = 100;
	fbci.useDepth = true;

	renderBuffer->init(&fbci);
	renderLayerShader->init();

}

void RenderLayer::release() {
	
	if (renderBuffer) {
		renderBuffer->release();
		renderBuffer = nullptr;
	}

	if (renderLayerShader) {
		renderLayerShader->release();
		renderLayerShader = nullptr;
	}

	delete this;
}

void RenderLayer::useLayer() {
	renderBuffer->bind();
	renderLayerShader->useShader();
}
