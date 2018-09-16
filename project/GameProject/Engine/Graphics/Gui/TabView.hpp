#ifndef TAB_VIEW_HPP
#define TAB_VIEW_HPP

/// Internal Includes
#include "ScrollBar.hpp"
#include "Button.hpp"
#include "ScrollArea.hpp"
#include "Panel.hpp"
#include "Text.hpp"
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class TabPanel : public Panel {

			public:
				TabPanel(GuiInfo& info);
				virtual ~TabPanel();

				void setName(const Engine::Core::String& name);
				Engine::Core::String getName() const;

				virtual bool isFocusable() const override;
				virtual bool hasFocusableItems() const override;

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				Engine::Core::String tabName;

			};

			class TabView : public GuiItem {
			public:

				TabView(GuiInfo& info);
				virtual ~TabView();

				void addTab(TabPanel* tab);

				virtual bool isFocusable() const override;
				virtual bool hasFocusableItems() const override;

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:
				
				Button* nextButton;
				Button* prevButton;
				Label* selectedTabName;

				ScrollArea* scrollArea;

				size_t selectedTab;
				std::vector<TabPanel*> tabs;

			};
		}
	}
}

#endif
