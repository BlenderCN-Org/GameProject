#include "GuiLabel.hpp"

#include <GameProject/Core/System/Sys.hpp>
#include "../../../Core/CoreGlobals.hpp"
#include "../Gui.hpp"

GuiLabel::GuiLabel() : text(nullptr) {
	textObject = new Text();
	VPMat = gGui->getDefaultTextShader()->getShaderUniform("viewProjMatrix");
	TransMat = gGui->getDefaultTextShader()->getShaderUniform("worldMat");
	textColor = gGui->getDefaultTextShader()->getShaderUniform("textColor");
	textSampler = gGui->getDefaultTextShader()->getShaderUniform("text");
}

GuiLabel::~GuiLabel() {
	if (text) {
		delete text;
		text = nullptr;
	}
	if (textObject) {
		delete textObject;
		textObject = nullptr;
	}
}

void GuiLabel::setText(const char * _text) {
	uint32_t strLength = getStringLength(_text);
	text = new char[strLength];
	memcpy((void*)text, _text, strLength);
}

void GuiLabel::render(glm::mat4 & vpMatRef) {

	glm::mat4 positionMatrix;

	calculatePoints(positionMatrix, vpMatRef);
	
	uint32_t strLength = getStringLength(text);
	gGui->getDefaultTextShader()->useShader();
	gGui->getDefaultTextShader()->bindData(VPMat, UniformDataType::UNI_MATRIX4X4, &gGui->cam->orthoMatrix);
	gGui->getDefaultTextShader()->bindData(TransMat, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	gGui->getDefaultTextShader()->bindData(textColor, UniformDataType::UNI_FLOAT3, &glm::vec3(1, 1, 1));
	int txture = 0;
	gGui->getDefaultTextShader()->bindData(textSampler, UniformDataType::UNI_INT, &txture);

	textObject->setText(text, strLength, 100, 100, 1);

	gRenderEngine->setBlending(true);
	textObject->render(gGui->getDefaultTextShader(), txture);
	gRenderEngine->setBlending(false);

}
