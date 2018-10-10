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
				Panel(GuiInfo& info);
				virtual ~Panel();
				
				void addGuiItem(GuiItem* guiItem);

				virtual bool isFocusable() const override;
				virtual bool hasFocusableItems() const override;
				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				std::vector<GuiItem*> subItems;
			};
		}
	}
}

#endif