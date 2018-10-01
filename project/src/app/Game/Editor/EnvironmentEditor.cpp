/// Internal Includes
#include "EnvironmentEditor.hpp"

/// External Includes

/// Std Includes


EnvironmentEditor::EnvironmentEditor(Engine::Graphics::GuiInfo& info) : Panel(info) {

	panelTitle = new Engine::Graphics::Gui::Label(info);

	panelTitle->setText("Environment Editor");
	panelTitle->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	panelTitle->setPosition(0, 20);
	panelTitle->setSize(panelTitle->calcTextWidth(), panelTitle->calcTextHeight());
	panelTitle->setVisible(true);

	landscapeTerraformTool = new Engine::Graphics::Gui::TabPanel(info);
	landscapeSmoothTool = new Engine::Graphics::Gui::TabPanel(info);
	landscapeSlopeTool = new Engine::Graphics::Gui::TabPanel(info);
	landscapePainTool = new Engine::Graphics::Gui::TabPanel(info);

	landscapeTerraformTool->setName("Terraform");
	landscapeTerraformTool->setSize(400, 1000);
	landscapeTerraformTool->setVisible(true);

	// @ TODO: Remove
	// @ MEMORY LEAK
	auto btn = new Engine::Graphics::Gui::Button(info);
	btn->setPosition(0, 0);
	btn->setSize(100, 50);
	btn->setAnchorPoint(Engine::Graphics::GuiAnchor::CENTER);
	btn->setVisible(true);
	btn->setText("Hello :)");

	landscapeTerraformTool->addGuiItem(btn);

	landscapeSmoothTool->setName("Smooth");
	landscapeSmoothTool->setSize(200, 200);
	landscapeSmoothTool->setVisible(true);
	
	landscapeSmoothTool->addGuiItem(btn);

	landscapeSlopeTool->setName("Slope");
	landscapeSlopeTool->setSize(200, 200);
	landscapeSlopeTool->setVisible(true);

	landscapePainTool->setName("Paint");
	landscapePainTool->setSize(200, 200);
	landscapePainTool->setVisible(true);

	tabView = new Engine::Graphics::Gui::TabView(info);
	tabView->setSize(100, 100);
	tabView->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	tabView->setPosition(0, 40);
	tabView->setVisible(true);

	tabView->addTab(landscapeTerraformTool);
	tabView->addTab(landscapeSmoothTool);
	tabView->addTab(landscapeSlopeTool);
	tabView->addTab(landscapePainTool);

	addGuiItem(panelTitle);
	addGuiItem(tabView);
}

EnvironmentEditor:: ~EnvironmentEditor() {
	delete panelTitle;
	delete tabView;

	delete landscapeTerraformTool;
	delete landscapeSmoothTool;
	delete landscapeSlopeTool;
	delete landscapePainTool;
}

bool EnvironmentEditor::isFocusable() const {
	return false;
}

bool EnvironmentEditor::hasFocusableItems() const {
	return false;
}

void EnvironmentEditor::update(float dt, Engine::Graphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) {

	tabView->setSize(size.x - 10, size.y - 40);
	Panel::update(dt, hitInfo, currentFocus);
}

void EnvironmentEditor::render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) {
	Panel::render(vpMatRef, shaderContainer);
}