
/// Internal Includes
#include "SkyEdit.hpp"

/// External Includes

/// Std Includes

const int32_t ITEM_SIZE = 20;

SkyEdit::SkyEdit(Engine::Graphics::GuiInfo& info)
	: Panel(info)
	, okButton(nullptr)
	, cancelButton(nullptr)
	, dayColor(nullptr)
	, nightColor(nullptr)
	, cycleTimeEdit(nullptr)
	, dayLabel(nullptr)
	, nightLabel(nullptr)
	, cycleLabel(nullptr)
	, currentId(0U)
	, oldData()
{

	setupComponents();

}

SkyEdit::~SkyEdit() {
	delete okButton;
	delete cancelButton;
	delete dayColor;
	delete nightColor;
	delete cycleTimeEdit;
	delete dayLabel;
	delete nightLabel;
	delete cycleLabel;

}

void SkyEdit::update(float dt, Engine::Graphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) {
	if (visible) {

		okButton->setSize(size.x - 4, ITEM_SIZE);
		cancelButton->setSize(size.x - 4, ITEM_SIZE);

		dayColor->setSize(size.x - 4, ITEM_SIZE);
		nightColor->setSize(size.x - 4, ITEM_SIZE);
		cycleTimeEdit->setSize(size.x - 4, ITEM_SIZE);

		dayLabel->setSize(size.x - 4, ITEM_SIZE);
		nightLabel->setSize(size.x - 4, ITEM_SIZE);
		cycleLabel->setSize(size.x - 4, ITEM_SIZE);

		Panel::update(dt, hitInfo, currentFocus);
	}
}

void SkyEdit::render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) {
	Panel::render(vpMatRef, shaderContainer);
}

void SkyEdit::setCurrentSky(uint32_t ) {


}

void SkyEdit::setupComponents() {

	okButton = new      Engine::Graphics::Gui::Button(guiInfo);
	cancelButton = new  Engine::Graphics::Gui::Button(guiInfo);

	dayColor = new      Engine::Graphics::Gui::Button(guiInfo);
	nightColor = new    Engine::Graphics::Gui::Button(guiInfo);
	cycleTimeEdit = new Engine::Graphics::Gui::TextArea(guiInfo);

	dayLabel = new      Engine::Graphics::Gui::Label(guiInfo);
	nightLabel = new    Engine::Graphics::Gui::Label(guiInfo);
	cycleLabel = new    Engine::Graphics::Gui::Label(guiInfo);

	okButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	cancelButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);

	dayColor->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	nightColor->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	cycleTimeEdit->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);

	dayLabel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	nightLabel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	cycleLabel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP);
	
	int pos = 2;

	dayLabel->setPosition(0, pos);
	dayColor->setPosition(0, pos += (ITEM_SIZE + 4) );

	nightLabel->setPosition(0, pos += (ITEM_SIZE + 4));
	nightColor->setPosition(0, pos += (ITEM_SIZE + 4));

	cycleLabel->setPosition(0, pos += (ITEM_SIZE + 4));
	cycleTimeEdit->setPosition(0, pos += (ITEM_SIZE + 4));

	okButton->setPosition(0, pos += (ITEM_SIZE + 4));
	cancelButton->setPosition(0, pos += (ITEM_SIZE + 4));

	okButton->setText("Ok");
	cancelButton->setText("Cancel");

	dayColor->setText("");
	nightColor->setText("");

	dayLabel->setText("Day Color");
	nightLabel->setText("Night Color");
	cycleLabel->setText("Cycle Time");

	cycleTimeEdit->setText("");

	cycleTimeEdit->showLineNumbers(false);
	cycleTimeEdit->setMultiLine(false);
	cycleTimeEdit->setEditMode(true);
	cycleTimeEdit->setOnlyNumbers(true);
	Engine::Core::FormattedString passiveTex = "Only numbers (0-9)";
	passiveTex.formatString(size_t(0), size_t(-1), glm::vec4(0.5F));
	cycleTimeEdit->setPassiveText(passiveTex);
	cycleTimeEdit->setSize(20, 20);

	okButton->setVisible(true);
	cancelButton->setVisible(true);

	dayColor->setVisible(true);
	nightColor->setVisible(true);
	cycleTimeEdit->setVisible(true);

	dayLabel->setVisible(true);
	nightLabel->setVisible(true);
	cycleLabel->setVisible(true);

	addGuiItem(okButton);
	addGuiItem(cancelButton);
	addGuiItem(dayColor);
	addGuiItem(nightColor);
	addGuiItem(cycleTimeEdit);
	addGuiItem(dayLabel);
	addGuiItem(nightLabel);
	addGuiItem(cycleLabel);

}