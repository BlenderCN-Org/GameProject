/// Internal Includes
#include "Panel.hpp"
#include "../Graphics.hpp"
#include "../../Input/Input.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			Panel::Panel(GuiInfo& info)
				: GuiItem(info)
				, subItems() {

			}

			Panel::~Panel() {

			}

			void Panel::addGuiItem(GuiItem* guiItem) {
				subItems.push_back(guiItem);
			}

			bool Panel::isFocusable() const {
				return false;
			}
			bool Panel::hasFocusableItems() const {

				bool subFocus = false;

				std::vector<GuiItem*>::const_reverse_iterator it = subItems.rbegin();
				std::vector<GuiItem*>::const_reverse_iterator eit = subItems.rend();

				for (it; it != eit && !subFocus; it++) {
					subFocus = (*it)->isFocusable();
				}

				for (it; it != eit && !subFocus; it++) {
					subFocus = (*it)->hasFocusableItems();
				}

				return subFocus;
			}

			void Panel::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {
				if (visible) {
					std::vector<GuiItem*>::reverse_iterator it = subItems.rbegin();
					std::vector<GuiItem*>::reverse_iterator eit = subItems.rend();

					for (it; it != eit; it++) {
						(*it)->updateAbsoultePos(absolutePosition.x, absolutePosition.y, absoluteSize.x, absoluteSize.y);
						(*it)->update(dt, hitInfo, currentFocus);
					}

					if (isMouseInside()) {
						hitInfo.mouseHit = true;
					}

				}
			}

			void Panel::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
				if (visible) {
					
					Theme::GuiTheme* theme = gTheme;

					if (nullptr != themeOverride) {
						theme = themeOverride;
					}

					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

					calculatePoints(vpMatRef);

					int textureSlot = 0;

					// setup shader
					shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);
					
					if (theme->panel.textureNormal) {
						theme->panel.textureNormal->bind();
						shaderContainer.standardQuad->bind();
						shaderContainer.standardQuad->render();
					}

					std::vector<GuiItem*>::iterator it = subItems.begin();
					std::vector<GuiItem*>::iterator eit = subItems.end();

					posAndSize = positionAndSizeFromMatrix(vpMatRef);
					// render all subitems
					for (it; it != eit; it++) {
						glm::mat4 cpy = vpMatRef;
						const ScissorInfo scinfo = gRenderEngine->pushScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)posAndSize.z, (int)posAndSize.w);
						(*it)->render(cpy, shaderContainer);
						gRenderEngine->popScissorRegion(scinfo);
					}
				}
			}

		} // namespace Gui
	} // namespace Graphics
} // namespace Engine

