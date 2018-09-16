/// Internal Includes
#include "CameraStatusInfo.hpp"
#include "../../Engine/Graphics/GuiTheme.hpp"
#include "../../Engine/Graphics/Graphics.hpp"

/// External Includes

/// Std Includes
CameraStatusInfo::CameraStatusInfo(EGraphics::GuiInfo& guiInfo)
	: IStatusItem(guiInfo)
	, mInfoLabel(nullptr)
	, mCamera(nullptr)
	, mCameraInput(nullptr) {
	mInfoLabel = new EGraphics::Gui::Label(guiInfo);
	mInfoLabel->setText("Pos (0.00, 0.00, 0.00)\n Dir (0.00, 0.00, 0.00)");
	mInfoLabel->setAnchorPoint(EGraphics::GuiAnchor::TOP);
	mInfoLabel->setVisible(true);
}

CameraStatusInfo::~CameraStatusInfo() {
	delete mInfoLabel;
}

void CameraStatusInfo::setCamera(Camera* cam, CameraInput* camInput) {
	mCamera = cam;
	mCameraInput = camInput;
}

void CameraStatusInfo::update(float dt, EGraphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) {

	if ((nullptr != mCamera) && (nullptr != mCameraInput)) {

		char buf[50];

		glm::vec3 pos = mCamera->getPos();
		glm::vec3 dir = mCameraInput->direction();
		snprintf(buf, sizeof(buf), "Pos (%.2f, %.2f, %.2f)\nDir (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z, dir.x, dir.y, dir.z);
		
		mInfoLabel->setText(buf);
	}
	mInfoLabel->update(dt, hitInfo, currentFocus);
	mInfoLabel->setSize(mInfoLabel->calcTextWidth(), mInfoLabel->calcTextHeight() + 5);
	mInfoLabel->updateAbsoultePos(absolutePosition.x, absolutePosition.y, size.x, size.y);
}

void CameraStatusInfo::render(glm::mat4 &vpMatRef, EGraphics::GuiShaderContainer& shaderContainer) {

	Engine::Theme::GuiTheme* theme = gTheme;

	if (nullptr != themeOverride) {
		theme = themeOverride;
	}

	glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

	calculatePoints(vpMatRef);

	int textureSlot = 0;

	// setup shader
	shaderContainer.guiElementShader->useShader();
	shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
	shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
	shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);

	if (theme->statusBar.textureBackground) {
		theme->statusBar.textureBackground->bind();
		shaderContainer.standardQuad->bind();
		shaderContainer.standardQuad->render();
	}

	mInfoLabel->render(vpMatRef, shaderContainer);

}