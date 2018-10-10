/// Internal Includes
#include "TabView.hpp"

#include "../GuiTheme.hpp"
#include "../Graphics.hpp"
/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			// Panel
			TabPanel::TabPanel(GuiInfo& info) : Panel(info) {

			}

			TabPanel::~TabPanel() {

			}

			void TabPanel::setName(const Engine::Core::String& name) {
				tabName = name;
			}

			Engine::Core::String TabPanel::getName() const {
				return tabName;
			}

			bool TabPanel::isFocusable() const {
				return false;
			}

			bool TabPanel::hasFocusableItems() const {
				return false;
			}

			void TabPanel::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {
				Panel::update(dt, hitInfo, currentFocus);
			}

			void TabPanel::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
				Panel::render(vpMatRef, shaderContainer);
			}

			// View
			TabView::TabView(GuiInfo& info) : GuiItem(info) {
				selectedTab = 0;

				scrollArea = new ScrollArea(info);

				nextButton = new Button(info);
				prevButton = new Button(info);

				selectedTabName = new Label(info);

				nextButton->setSize(34, 34);
				nextButton->setPosition(-10, 10);
				nextButton->setAnchorPoint(GuiAnchor::TOP_RIGHT);
				nextButton->setVisible(true);
				nextButton->setText("");
				nextButton->setIcon(gTheme->tabView.icoNext);

				prevButton->setSize(34, 34);
				prevButton->setPosition(10, 10);
				prevButton->setAnchorPoint(GuiAnchor::TOP_LEFT);
				prevButton->setVisible(true);
				prevButton->setText("");
				prevButton->setIcon(gTheme->tabView.icoPrev);

				selectedTabName->setPosition(0, 10);
				selectedTabName->setAnchorPoint(GuiAnchor::TOP);
				selectedTabName->setVisible(true);

				scrollArea->setVisible(true);
				scrollArea->setAnchorPoint(GuiAnchor::TOP);
				scrollArea->setPosition(0, 50);
				scrollArea->setSize(20, 20);
			}

			TabView::~TabView() {
				delete scrollArea;
				delete nextButton;
				delete prevButton;
				delete selectedTabName;
			}

			void TabView::addTab(TabPanel* tab) {
				tabs.push_back(tab);
			}

			bool TabView::isFocusable() const {
				return false;
			}

			bool TabView::hasFocusableItems() const {
				return false;
			}

			void TabView::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {
				if (visible) {

					nextButton->updateAbsoultePos(absolutePosition.x, absolutePosition.y, size.x, size.y);
					nextButton->update(dt, hitInfo, currentFocus);

					prevButton->updateAbsoultePos(absolutePosition.x, absolutePosition.y, size.x, size.y);
					prevButton->update(dt, hitInfo, currentFocus);

					scrollArea->setSize(size.x - 4, size.y - 52);

					if (tabs.size() > 0) {

						if (tabs.size() > 1) {
							nextButton->setEnabled(true);
							prevButton->setEnabled(true);
							if (nextButton->wasPressed()) {
								selectedTab++;
								selectedTab = selectedTab % tabs.size();
							} else if (prevButton->wasPressed()) {
								selectedTab--;
								selectedTab = selectedTab % tabs.size();
							}
						} else {
							nextButton->setEnabled(false);
							prevButton->setEnabled(false);
						}

						selectedTabName->setText(tabs[selectedTab]->getName());
						selectedTabName->setSize(selectedTabName->calcTextWidth(), selectedTabName->calcTextHeight());

						selectedTabName->updateAbsoultePos(absolutePosition.x, absolutePosition.y, size.x, size.y);
						selectedTabName->update(dt, hitInfo, currentFocus);

						scrollArea->setChild(tabs[selectedTab]);
						scrollArea->updateAbsoultePos(absolutePosition.x, absolutePosition.y, size.x, size.y);
						scrollArea->update(dt, hitInfo, currentFocus);
					}
				}
			}

			void TabView::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {

				if (visible) {

					nextButton->render(vpMatRef, shaderContainer);
					prevButton->render(vpMatRef, shaderContainer);

					selectedTabName->render(vpMatRef, shaderContainer);
					if (tabs.size() > 0) {

						scrollArea->render(vpMatRef, shaderContainer);

					}
				}
			}
		}
	}
}