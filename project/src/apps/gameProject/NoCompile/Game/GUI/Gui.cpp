#include "Gui.hpp"
#include "DefaultTextShader.hpp"

#include "../../Core/CoreGlobals.hpp"
#include <RenderEngine/IRenderEngine.hpp>
#include "../../Core/System/Console.hpp"

void Gui::init() {
	cam = nullptr;
	gGui = this;

	font = gRenderEngine->createFont();
	textShader = gRenderEngine->createShaderObject();
	pointQuadShader = gRenderEngine->createShaderObject();
	defaultQuad = gRenderEngine->createMesh();

	font->init("C:/Windows/Fonts/Arial.ttf", 12);
	textShader->init();
	pointQuadShader->init();
	defaultQuad->init(MeshPrimitiveType::POINT);

	textShader->setShaderCode(ShaderStages::VERTEX_STAGE, defaultTextVertexShaderCode);
	textShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	textShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, defaultTextFragmentShaderCode);

	if (textShader->buildShader() == false) {
		gConsole->print("Failed to build default text shader\n");
	}

	pointQuadShader->setShaderCode(ShaderStages::VERTEX_STAGE, defaultPassthroughVertexShaderCode);
	pointQuadShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, defaultPointToQuadGeomentryShaderCode);
	pointQuadShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, defaultTextFragmentShaderCode);

	if (pointQuadShader->buildShader() == false) {
		gConsole->print("Failed to build default quad to point shader\n");
	}

	float data[5];
	memset(data, 0, sizeof(data));
	defaultQuad->setMeshData(data, sizeof(data), VERT_UV);
}

void Gui::freeResources() {
	font->release();
	textShader->release();
	pointQuadShader->release();
	defaultQuad->release();

	font = nullptr;
	textShader = nullptr;
	defaultQuad = nullptr;

	gGui = nullptr;
}

IReFont * Gui::getDefaultFont() {
	return font;
}

IShaderObject * Gui::getDefaultTextShader() {
	return textShader;
}