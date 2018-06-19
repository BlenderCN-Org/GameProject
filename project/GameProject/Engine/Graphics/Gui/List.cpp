/// Internal Includes
#include "List.hpp"
#include "../Graphics.hpp"
#include "../../Input/Input.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			List::List() {
			
				verticalScroll = new ScrollBar();
				verticalScroll->setAnchorPoint(GuiAnchor::RIGHT);
				verticalScroll->setScrollDirection(ScrollBarDirection::SCROLL_VERTICAL);
				verticalScroll->setVisible(true);
				verticalScroll->setBackgroundTexture(nullptr);
				verticalScroll->setScrollbarTexture(nullptr);
				verticalScroll->setAutoScroll(false);
			}
			List::~List() {
				delete verticalScroll;
			}

			void List::setTexture(Texture::Texture2D* texture) {
				tex = texture;
			}

			void List::addListItem(ListItem* listItem) {
				listItems.push_back(listItem);
			}

			void List::search(Core::String str, ListSearchFunction searchFunc) {

				if (0 == str.getSize() || nullptr == searchFunc) {
					// @TODO clear the resulting list
					return;
				}



			}

			void List::update(float dt, GuiHitInfo& hitInfo) {
				if (visible) {
					std::vector<ListItem*>::iterator it = listItems.begin();
					std::vector<ListItem*>::iterator eit = listItems.end();

					for (it; it != eit; it++) {
						(*it)->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x, size.y);
						(*it)->update(dt, hitInfo);
					}

					verticalScroll->setSize(15, size.y);

				}
			}

			void List::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
				if (visible) {
					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

					calculatePoints(vpMatRef);

					int textureSlot = 0;

					// setup shader
					shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);
					if (tex) {
						tex->bind();
					}

					shaderContainer.standardQuad->bind();
					shaderContainer.standardQuad->render();

					/*
					std::vector<GuiItem*>::iterator it = listItems.begin();
					std::vector<GuiItem*>::iterator eit = listItems.end();

					posAndSize = positionAndSizeFromMatrix(vpMatRef);
					// render all subitems
					for (it; it != eit; it++) {
						glm::mat4 cpy = vpMatRef;
						gRenderEngine->setScissorTest(true);
						gRenderEngine->setScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)posAndSize.z, (int)posAndSize.w);
						(*it)->render(cpy, shaderContainer);
						gRenderEngine->setScissorTest(false);
					}
					*/
				}
			}
		}
	}
}