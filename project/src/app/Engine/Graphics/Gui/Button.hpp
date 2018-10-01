#ifndef BUTTON_HPP
#define BUTTON_HPP

/// Internal Includes
#include "Label.hpp"
#include "Text.hpp"
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {
			class Button : public GuiItem {

			public:

				Button(GuiInfo& info);
				virtual ~Button();
				
				bool wasPressed();

				void setEnabled(bool enable);
				bool getEnabled() const;

				void setIcon(Engine::Graphics::Texture::Texture2D* ico);

				void setText(const Engine::Core::FormattedString& str);

				virtual bool isFocusable() const override;
				virtual bool hasFocusableItems() const override;
				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:
				
				Label* lbl;

				bool hovering;
				bool pressing;
				bool clicked;

				bool enabled;

			};
		}
	}
}
#endif