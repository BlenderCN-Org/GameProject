///Internal Includes

#include "RenderBatch.hpp"
#include "Sky.hpp"

#include "../Engine/Graphics/Graphics.hpp"


ActiveFrameBufferCommand::ActiveFrameBufferCommand(IFrameBuffer* fbo, bool clear) : activeFbo(fbo), cls(clear) {}

void ActiveFrameBufferCommand::execute() {
	if (activeFbo) {
		activeFbo->bind();
		if (cls) {
			activeFbo->clear();
		}
	} else {
		gRenderEngine->bindDefaultFrameBuffer();
	}
}

ActiveShaderCommand::ActiveShaderCommand(IShaderObject* shdr) : shader(shdr) {}

void ActiveShaderCommand::execute() {
	if (shader) {
		shader->useShader();
	} else {
		throw "No shader being activated, command is not valid";
	}
}

ViewProjectionCommand::ViewProjectionCommand(IShaderObject* shader, int32_t viewProjectionLocation, glm::mat4* vpMatrix)
	: boundShader(shader)
	, vpLoc(viewProjectionLocation)
	, vpMat(vpMatrix) {}

void ViewProjectionCommand::execute() {
	if (boundShader) {
		boundShader->bindData(vpLoc, UniformDataType::UNI_MATRIX4X4, vpMat);
	} else {
		throw "Shader is invalid";
	}
}

SkyCommand::SkyCommand(Sky* _sky, glm::mat4 _vpMat, glm::vec3 _camPos, glm::vec3 _camDir, glm::mat4* _reflect)
	: sky(_sky)
	, vpMat(_vpMat)
	, camPos(_camPos)
	, camDir(_camDir)
	, reflect(_reflect) {}

void SkyCommand::execute() {
	sky->render(vpMat, camPos, camDir, reflect);
}

RenderBatch::RenderBatch(size_t maxCommands, TemporaryStorage* tempStorage) {

	maxRenderCommands = maxCommands;
	numRenderCommands = 0U;

	renderCommands = tempStorage->allocateNoNew<RenderCommand*>(maxRenderCommands);

}

void RenderBatch::addCommand(RenderCommand* command) {

	renderCommands[numRenderCommands] = command;
	numRenderCommands++;

}

void RenderBatch::executeBatch() {

	for (size_t i = 0; i < numRenderCommands; i++) {
		renderCommands[i]->execute();
	}

}