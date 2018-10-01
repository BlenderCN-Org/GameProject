#ifndef ENVIRONMENT_EDITOR_HPP
#define ENVIRONMENT_EDITOR_HPP

/// Internal Includes
#include "../../Engine/Graphics/GuiItem.hpp"
#include "../../Engine/Graphics/ShaderContainer.hpp"

#include "../../Engine/Graphics/GuiItem.hpp"
#include "../../Engine/Graphics/Gui/Label.hpp"
#include "../../Engine/Graphics/Gui/Panel.hpp"
#include "../../Engine/Graphics/Gui/TabView.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <vector>

class EnvironmentEditor : public Engine::Graphics::Gui::Panel {

public:

	EnvironmentEditor(Engine::Graphics::GuiInfo& info);
	virtual ~EnvironmentEditor();

	virtual bool isFocusable() const override;
	virtual bool hasFocusableItems() const override;

	virtual void update(float dt, Engine::Graphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
	virtual void render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) override;

private:

	Engine::Graphics::Gui::Label* panelTitle;
	Engine::Graphics::Gui::TabView* tabView;

	Engine::Graphics::Gui::TabPanel* landscapeTerraformTool;
	Engine::Graphics::Gui::TabPanel* landscapeSmoothTool;
	Engine::Graphics::Gui::TabPanel* landscapeSlopeTool;
	Engine::Graphics::Gui::TabPanel* landscapePainTool;

};

#endif