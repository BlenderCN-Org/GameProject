/// Internal Includes
#include "ToolSelector.hpp"
#include "../../Engine/Graphics/Graphics.hpp"
#include "../../Engine/Graphics/GuiTheme.hpp"
#include "../../Engine/Input/Input.hpp"

#include "Resources/SelectorGradient.png.h"

/// External Includes
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <AssetLib/AssetLib.hpp>

/// Std Includes

const uint32_t RADIAL_SIZE = 200;

ToolSelector::ToolSelector(Engine::Graphics::GuiInfo& info) : GuiItem(info) {
	radialMesh = gRenderEngine->createMesh();
	radialMesh->init(MeshPrimitiveType::TRIANGLE_FAN);

	struct Vert {
		float x, y, z, u, v;
		float r, g, b, a;
	};
	const uint32_t numVerts = 64;
	Vert data[numVerts + 2];

	const float TWO_PI = glm::pi<float>() * 2;

	data[0] = { 0, 0, 0, 0, 0, 0.5F, 0.5F, 0.5F, 1.0F };

	for (uint32_t i = 0; i < numVerts; i++) {

		float x = glm::sin((float(i) / float(numVerts)) * TWO_PI);
		float y = glm::cos((float(i) / float(numVerts)) * TWO_PI);

		float fPosX = (x * RADIAL_SIZE);
		float fPosY = (y * RADIAL_SIZE);

		data[i + 1] = { fPosX, fPosY, 0, 1, 0, 0.5F, 0.5F, 0.5F, 1.0F };
	}

	data[numVerts + 1] = { 0, float(RADIAL_SIZE), 0, 1, 0, 0.5F, 0.5F, 0.5F, 1.0F };

	radialMesh->setMeshData(data, sizeof(data), MeshDataLayout::VERT_UV_COL);

	int w, h, c;

	void* textureData = AssetLib::loadTextureFromMemory((void*)selectorGradient_32x1, sizeof(selectorGradient_32x1), w, h, c);

	backgroundTexture = new Engine::Graphics::Texture::Texture2D();
	backgroundTexture->setData(w, h, c, textureData);

	AssetLib::freeImageData(textureData);
}

ToolSelector::~ToolSelector() {
	radialMesh->release();
	delete backgroundTexture;
}

bool ToolSelector::isFocusable() const {
	return false;
}

bool ToolSelector::hasFocusableItems() const {
	return false;
}

void ToolSelector::update(float , Engine::Graphics::GuiHitInfo& , GuiItem* ) {}

void ToolSelector::render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) {

	if (visible) {

		glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

		calculatePoints(vpMatRef);

		int textureSlot = 0;

		if (backgroundTexture) {
			backgroundTexture->bind();
		}

		shaderContainer.guiTextShader->useShader();
		shaderContainer.guiTextShader->bindData(shaderContainer.textVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
		shaderContainer.guiTextShader->bindData(shaderContainer.textTransform, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
		shaderContainer.guiTextShader->bindData(shaderContainer.textTexture, UniformDataType::UNI_INT, &textureSlot);

		//gRenderEngine->setScissorTest(true);
		//gRenderEngine->setScissorRegion(absoulutePosition.x, absoulutePosition.y, size.x, size.y);

		radialMesh->bind();
		radialMesh->render();

		//gRenderEngine->setScissorTest(false);
	}
}

void ToolSelector::clearTools() {
	selectableTools.clear();
}

void ToolSelector::addTool(ToolInfo& tool) {
	selectableTools.push_back(tool);
}