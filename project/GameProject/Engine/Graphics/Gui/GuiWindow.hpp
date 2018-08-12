#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

/// Internal Includes
#include "Button.hpp"
#include "Panel.hpp"
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class GuiWindow : public GuiItem {

			public:
				GuiWindow(GuiInfo& info);
				virtual ~GuiWindow();

				void addGuiItem(GuiItem* item);

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus);
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