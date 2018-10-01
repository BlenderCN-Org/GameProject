#ifndef SKY_EDIT_HPP
#define SKY_EDIT_HPP

/// Internal Includes
#include "../../../Engine/Graphics/GuiItem.hpp"
#include "../../../Engine/Graphics/ShaderContainer.hpp"

#include "../../../Engine/Graphics/Gui/Panel.hpp"
#include "../../../Engine/Graphics/Gui/Button.hpp"
#include "../../../Engine/Graphics/Gui/TextArea.hpp"


#include "../../MapObjects/Sky.hpp"

/// External Includes
#include <AssetHandling/IAssetManager.hpp>
#include <AssetHandling/Loader/IEditLoader.hpp>

/// Std Includes

class SkyEdit : public Engine::Graphics::Gui::Panel{

public:
	SkyEdit(Engine::Graphics::GuiInfo& info);
	virtual ~SkyEdit();

	virtual void update(float dt, Engine::Graphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
	virtual void render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) override;

	void setCurrentSky(uint32_t id);

private:

	void setupComponents();

	uint32_t currentId;

	SkyData oldData;

	// Gui elements
	Engine::Graphics::Gui::Button* okButton;
	Engine::Graphics::Gui::Button* cancelButton;

	Engine::Graphics::Gui::Button* dayColor;
	Engine::Graphics::Gui::Button* nightColor;
	Engine::Graphics::Gui::TextArea* cycleTimeEdit;

	Engine::Graphics::Gui::Label* dayLabel;
	Engine::Graphics::Gui::Label* nightLabel;
	Engine::Graphics::Gui::Label* cycleLabel;

};

#endif