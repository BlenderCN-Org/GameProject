#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "Button.hpp"
#include "Panel.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class GuiWindow : public GuiItem {

			public:
				GuiWindow();
				virtual ~GuiWindow();

				void addGuiItem(GuiItem* item);

				virtual void update(float dt);
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer);

			private:

				int oldX;
				int oldY;

				bool updating;

				std::vector<GuiItem*> subItems;
				Panel* itemPanel;

				Texture::Texture2D* background;
				Texture::Texture2D* titleBar;
				Button* closeButton;

				Texture::Texture2D* buttonTexture;
				Texture::Texture2D* buttonHover;
				Texture::Texture2D* buttonPress;

			};

		}
	}
}

#endif