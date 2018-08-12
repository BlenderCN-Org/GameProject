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

				void setTexture(Texture::Texture2D* texture);
				void setHoverTexture(Texture::Texture2D* texture);
				void setPressTexture(Texture::Texture2D* texture);

				void setText(const Engine::Core::FormattedString& str);

				virtual bool isFocusable() const override;
				virtual bool hasFocusableItems() const override;
				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:
				
				Texture::Texture2D* tex;
				Texture::Texture2D* hovTex;
				Texture::Texture2D* pressTex;

				Label* lbl;

				bool hovering;
				bool pressing;
				bool clicked;

			};
		}
	}
}
#endif