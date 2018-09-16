#ifndef SCROLL_AREA_HPP
#define SCROLL_AREA_HPP

/// Internal Includes
#include "ScrollBar.hpp"
#include "../GuiItem.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class ScrollArea : public GuiItem {
			public:

				ScrollArea(GuiInfo& info);
				virtual ~ScrollArea();

				void setChild(GuiItem* item);

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus);
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer);

			private:

				ScrollBar* vertBar;
				ScrollBar* horzBar;

				GuiItem* child;

				int totalWidth;
				int totalHeight;

				int32_t offsetX;
				int32_t offsetY;

				int oldX;
				int oldY;

			};
		}
	}
}

#endif