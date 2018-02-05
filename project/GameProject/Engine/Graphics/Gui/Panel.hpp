#ifndef PANEL_HPP
#define PANEL_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {
			class Panel : public GuiItem {
			public:
				Panel();
				virtual ~Panel();
				
				void setTexture(Texture::Texture2D* texture);

				void addGuiItem(GuiItem* guiItem);

				virtual void update(float dt) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				std::vector<GuiItem*> subItems;
				Texture::Texture2D* tex;
			};
		}
	}
}

#endif