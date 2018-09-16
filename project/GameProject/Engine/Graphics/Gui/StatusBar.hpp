#ifndef STATUS_BAR_HPP
#define STATUS_BAR_HPP

/// Internal Includes
#include "Panel.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class IStatusItem : public GuiItem {
			public:
				IStatusItem(GuiInfo& info) : GuiItem(info) {};
				virtual ~IStatusItem() {}

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) = 0;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) = 0;

			};

			class StatusBar : public GuiItem {

			public:
				StatusBar(GuiInfo& info);
				virtual ~StatusBar();

				void addStatusItem(IStatusItem* item);

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				std::vector<IStatusItem*> statusItems;

			};

		}
	}
}

#endif