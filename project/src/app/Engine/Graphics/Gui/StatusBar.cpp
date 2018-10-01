/// Internal Includes
#include "StatusBar.hpp"
#include "../Graphics.hpp"
#include "../../Input/Input.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			StatusBar::StatusBar(GuiInfo& info) : GuiItem(info) {
			
			}

			StatusBar::~StatusBar() {
			
			}

			void StatusBar::addStatusItem(IStatusItem* item) {
				statusItems.push_back(item);
			}

			void StatusBar::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {
				std::vector<IStatusItem*>::reverse_iterator it = statusItems.rbegin();
				std::vector<IStatusItem*>::reverse_iterator eit = statusItems.rend();

				for (it; it != eit; it++) {
					(*it)->updateAbsoultePos(absolutePosition.x, absolutePosition.y + 1, size.x, size.y - 2);
					(*it)->update(dt, hitInfo, currentFocus);
				}

				if (isMouseInside()) {
					hitInfo.mouseHit = true;
				}
			}

			void StatusBar::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
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

				if (theme->statusBar.textureBackground) {
					theme->statusBar.textureBackground->bind();
					shaderContainer.standardQuad->bind();
					shaderContainer.standardQuad->render();
				}

				std::vector<IStatusItem*>::iterator it = statusItems.begin();
				std::vector<IStatusItem*>::iterator eit = statusItems.end();

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
	}
}