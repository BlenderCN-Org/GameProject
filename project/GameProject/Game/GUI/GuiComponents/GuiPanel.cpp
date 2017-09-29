#include "GuiPanel.hpp"

#include "../../../Core/CoreGlobals.hpp"
#include "../Gui.hpp"
#include "../../../Core/Input/Input.hpp"

GuiPanel::GuiPanel() : GuiItem() {
	VPMat = gGui->pointQuadShader->getShaderUniform("viewProjMatrix");
	TransMat = gGui->pointQuadShader->getShaderUniform("worldMat");
}

GuiPanel::~GuiPanel() {}

void GuiPanel::addGuiItem(GuiItem item) {
	subItems.push_back(item);
}

void GuiPanel::setPanelMaterial(IMaterial * material) {
	panelMaterial = material;
}

void GuiPanel::render(glm::mat4 &vpMatRef) {
	
	int x = 0;
	int y = 0;

	glm::mat4 positionMatrix;

	if (anchorPoint == GuiAnchor::CENTER) {
		Input::getInput()->getWindowSize(x, y);
		x /= 2;
		y /= 2;

		glm::vec4 p0 = glm::vec4(x - size.x, y - size.y, 0, 0);
		glm::vec4 p1 = glm::vec4(x - size.x, y + size.y, 0, 0);
		glm::vec4 p2 = glm::vec4(x + size.x, y + size.y, 0, 0);
		glm::vec4 p3 = glm::vec4(x + size.x, y - size.y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	}
	else if (anchorPoint == GuiAnchor::TOP) {
		Input::getInput()->getWindowSize(x, y);
		x /= 2;
		y = 0;

		glm::vec4 p0 = glm::vec4(x - size.x, y             , 0, 0);
		glm::vec4 p1 = glm::vec4(x - size.x, y + 2 * size.y, 0, 0);
		glm::vec4 p2 = glm::vec4(x + size.x, y + 2 * size.y, 0, 0);
		glm::vec4 p3 = glm::vec4(x + size.x, y             , 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	}

	gGui->pointQuadShader->useShader();
	gGui->pointQuadShader->bindData(VPMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
	gGui->pointQuadShader->bindData(TransMat, UniformDataType::UNI_MATRIX4X4, &positionMatrix);
	
	gGui->defaultQuad->bind();
	gGui->defaultQuad->render();

	std::vector<GuiItem>::iterator it = subItems.begin();
	std::vector<GuiItem>::iterator eit = subItems.end();

	// render all subitems
	for (it; it != eit; it++) {
		it->render(vpMatRef);
	}
}
