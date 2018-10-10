#ifndef CAMERA_STATUS_INFO_HPP
#define CAMERA_STATUS_INFO_HPP

/// Internal Includes
#include "../../Engine/Graphics/Gui/StatusBar.hpp"
#include "../../Engine/Graphics/Gui/Label.hpp"
#include "../Camera.hpp"
#include "../CameraInput.hpp"

/// External Includes

/// Std Includes


namespace EGraphics = Engine::Graphics;

class CameraStatusInfo : public EGraphics::Gui::IStatusItem {

public:
	
	CameraStatusInfo(EGraphics::GuiInfo& guiInfo);
	virtual ~CameraStatusInfo();

	void setCamera(Camera* cam, CameraInput* camInput);

	virtual void update(float dt, EGraphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
	virtual void render(glm::mat4 &vpMatRef, EGraphics::GuiShaderContainer& shaderContainer) override;

private:
	EGraphics::Gui::Label* mInfoLabel;
	Camera* mCamera;
	CameraInput* mCameraInput;
};

#endif