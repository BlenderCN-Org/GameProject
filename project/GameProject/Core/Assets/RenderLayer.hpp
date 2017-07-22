#ifndef RENDERLAYER_HPP
#define RENDERLAYER_HPP

#include <RenderEngine/IRenderEngine.hpp>

#include <cstdint>

class RenderLayer {

public:

	void init(IRenderEngine* re);
	void release();

	void useLayer();

private:

	bool defaultLayer;

	IFrameBuffer* renderBuffer;

	int32_t nrShaderInputs;

	IShaderObject* renderLayerShader;

};

#endif