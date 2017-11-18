/// Internal Includes
#include "Panel.hpp"
#include "../Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			Panel::Panel()
				: subItems()
				, tex(nullptr) {

			}

			Panel::~Panel() {

			}

			void Panel::setTexture(Texture::Texture2D* texture) {
				tex = texture;
			}

			void Panel::addGuiItem(GuiItem* guiItem) {
				subItems.push_back(guiItem);
			}

			void Panel::update(float dt) {
				if (visible) {
					std::vector<GuiItem*>::iterator it = subItems.begin();
					std::vector<GuiItem*>::iterator eit = subItems.end();

					for (it; it != eit; it++) {
						(*it)->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x, size.y);
						(*it)->update(dt);
					}

				}
			}

			void Panel::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
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

					std::vector<GuiItem*>::iterator it = subItems.begin();
					std::vector<GuiItem*>::iterator eit = subItems.end();

					posAndSize = positionAndSizeFromMatrix(vpMatRef);
					// render all subitems
					for (it; it != eit; it++) {
						glm::mat4 cpy = vpMatRef;
						gRenderEngine->setScissorTest(true);
						gRenderEngine->setScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)posAndSize.z, (int)posAndSize.w);
						(*it)->render(cpy, shaderContainer);
						gRenderEngine->setScissorTest(false);
					}
				}
			}

		} // namespace Gui
	} // namespace Graphics
} // namespace Engine

