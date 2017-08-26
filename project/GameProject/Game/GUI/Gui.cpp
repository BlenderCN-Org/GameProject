#include "Gui.hpp"
#include "DefaultTextShader.hpp"

#include "../../Core/CoreGlobals.hpp"
#include <RenderEngine/IRenderEngine.hpp>
#include "../../Core/System/Console.hpp"

void Gui::init() {
	gGui = this;

	font = gRenderEngine->createFont();
	textShader = gRenderEngine->createShaderObject();

	font->init("C:/Windows/Fonts/Arial.ttf", 12);
	textShader->init();

	textShader->setShaderCode(ShaderStages::VERTEX_STAGE, defaultTextVertexShaderCode);
	textShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	textShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, defaultTextFragmentShaderCode);

	if (textShader->buildShader() == false) {
		gConsole->print("Failed to build default text shader\n");
	}
}

void Gui::freeResources() {
	font->release();
	textShader->release();

	font = nullptr;
	textShader = nullptr;

	gGui = nullptr;
}

IReFont * Gui::getDefaultFont() {
	return font;
}

IShaderObject * Gui::getDefaultTextShader() {
	return textShader;
}