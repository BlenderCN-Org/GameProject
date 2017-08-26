#include "Menu.hpp"

#include "../Core/System/Sys.hpp"

#include "../Core/CoreGlobals.hpp"
#include "GUI/Gui.hpp"

Menu::Menu() {
	maxMenuItems = 0;
	selectedMenuItem = 0;
	textureLocation = 0;
	colorLocation = 0;
	vpLocation = 0;
	mdlLocation = 0;
	menuItems.clear();

	IShaderObject* shader = gGui->getDefaultTextShader();

	if (textureLocation == 0) {
		textureLocation = shader->getShaderUniform("text");
	}

	if (colorLocation == 0) {
		colorLocation = shader->getShaderUniform("textColor");
	}

	if (vpLocation == 0) {
		vpLocation = shader->getShaderUniform("viewProjMatrix");
	}

	if (mdlLocation == 0) {
		mdlLocation = shader->getShaderUniform("worldMat");
	}
}

uint8_t Menu::addItem(uint32_t x, uint32_t y, char * text) {
	MenuItem mi;
	mi.x = x;
	mi.y = y;
	mi.text = text;
	mi.textLength = getStringLength(text);

	uint8_t idx = (uint8_t)menuItems.size();

	menuItems.push_back(mi);
	maxMenuItems++;

	return idx;
}

uint8_t Menu::getSelectedMenuItem() {
	return selectedMenuItem;
}

void Menu::setOrthoMatrix(glm::mat4 * ortho) {
	orthoPersp = ortho;
}

void Menu::selectNext() {
	selectedMenuItem = (selectedMenuItem + 1) % maxMenuItems;
}

void Menu::selectPrev() {
	selectedMenuItem = (selectedMenuItem == 0 ? maxMenuItems - 1 : selectedMenuItem - 1);
}

void Menu::render() {
	IShaderObject* shader = gGui->getDefaultTextShader();

	shader->useShader();

	shader->bindData(vpLocation, UniformDataType::UNI_MATRIX4X4, orthoPersp);
	shader->bindData(mdlLocation, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	gRenderEngine->setBlending(true);
	for (size_t i = 0; i < maxMenuItems; i++) {
		const MenuItem& mi = menuItems[i];
		if (selectedMenuItem == i) {
			shader->bindData(colorLocation, UniformDataType::UNI_FLOAT3, &glm::vec3(1.0F, 0.0F, 0.0F));
		} else {
			shader->bindData(colorLocation, UniformDataType::UNI_FLOAT3, &glm::vec3(1.0F));
		}
		textObj.setText(mi.text, mi.textLength, (float)mi.x, (float)mi.y, 1.0F);
		textObj.render(shader, textureLocation);
	}

	gRenderEngine->setBlending(false);
}