#include "GuiPanel.hpp"

#include "../../../Core/CoreGlobals.hpp"
#include "../Gui.hpp"
#include "../../../Core/Input/Input.hpp"

GuiPanel::GuiPanel() : GuiItem() {
	VPMat = gGui->pointQuadShader->getShaderUniform("viewProjMatrix");
	TransMat = gGui->pointQuadShader->getShaderUniform("worldMat");
}

GuiPanel::~GuiPanel() {}

void GuiPanel::addGuiItem(GuiItem* item) {
	subItems.push_back(item);
}

void GuiPanel::setPanelMaterial(IMaterial * material) {
	panelMaterial = material;
}

void GuiPanel::render(glm::mat4 &vpMatRef) {
	
	glm::mat4 positionMatrix;

	calculatePoints(positionMatrix);

	gGui->pointQuadShader->useShader();
	gGui->pointQuadShader->bindData(VPMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
	gGui->pointQuadShader->bindData(TransMat, UniformDataType::UNI_MATRIX4X4, &positionMatrix);
	
	gGui->defaultQuad->bind();
	gGui->defaultQuad->render();

	std::vector<GuiItem*>::iterator it = subItems.begin();
	std::vector<GuiItem*>::iterator eit = subItems.end();

	// render all subitems
	for (it; it != eit; it++) {
		(*it)->render(vpMatRef);
	}
}
