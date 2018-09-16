/// Internal Includes
#include "GuiConstants.hpp"
#include "ScrollArea.hpp"
#include "../Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			ScrollArea::ScrollArea(GuiInfo& info) : GuiItem(info), child(nullptr) {

				vertBar = new ScrollBar(info);
				vertBar->setAnchorPoint(GuiAnchor::RIGHT);
				vertBar->setScrollDirection(ScrollBarDirection::SCROLL_VERTICAL);
				vertBar->setVisible(true);
				vertBar->setAutoScroll(false);
				vertBar->setPosition(0, -SCROLLBAR_HALF_SIZE);
				vertBar->setSize(10, 10);

				horzBar = new ScrollBar(info);
				horzBar->setAnchorPoint(GuiAnchor::BOTTOM);
				horzBar->setScrollDirection(ScrollBarDirection::SCROLL_HORIZONTAL);
				horzBar->setVisible(true);
				horzBar->setAutoScroll(false);
				horzBar->setPosition(-SCROLLBAR_HALF_SIZE, 0);
				horzBar->setSize(10, 10);

			}

			ScrollArea::~ScrollArea() {
				delete vertBar;
				delete horzBar;
			}

			void ScrollArea::setChild(GuiItem* item) {
				child = item;

				if (child) {
					child->getSize(totalWidth, totalHeight);
					child->setAnchorPoint(GuiAnchor::TOP_LEFT);
					child->setVisible(true);
				}

				horzBar->setNumElements(totalWidth);
				vertBar->setNumElements(totalHeight);

			}

			void ScrollArea::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {
				if (visible) {

					vertBar->setMinElements(size.y - SCROLLBAR_SIZE);
					horzBar->setMinElements(size.x - SCROLLBAR_SIZE);

					vertBar->setSize(SCROLLBAR_SIZE, size.y - SCROLLBAR_SIZE);
					horzBar->setSize(size.x - SCROLLBAR_SIZE, SCROLLBAR_SIZE);

					vertBar->updateAbsoultePos(absolutePosition.x, absolutePosition.y, size.x, size.y);
					horzBar->updateAbsoultePos(absolutePosition.x, absolutePosition.y, size.x, size.y);

					vertBar->update(dt, hitInfo, currentFocus);
					horzBar->update(dt, hitInfo, currentFocus);

					//Input::Input* in = Input::Input::GetInput();
					//int x = 0, y = 0;
					//in->getMousePos(x, y);
					//
					//if (posInItem(x, y)) {
					//	if (in->isKeyBindPressed(Input::KeyBindings[Input::KEYBIND_MOUSE_WHEEL_CLICK])) {
					//
					//		int32_t elementX = horzBar->getSelectedElement();
					//		int32_t elementY = vertBar->getSelectedElement();
					//
					//		elementX -= (x - oldX);
					//		elementY -= (y - oldY);
					//
					//		horzBar->selectElement(elementX);
					//		vertBar->selectElement(elementY);
					//	}
					//
					//	oldX = x;
					//	oldY = y;
					//}

					offsetY = vertBar->getSelectedElement();
					offsetX = horzBar->getSelectedElement();

					if (nullptr != child) {
						child->setPosition(-offsetX, -offsetY);

						child->updateAbsoultePos(absolutePosition.x, absolutePosition.y, absoluteSize.x, absoluteSize.y);
						child->update(dt, hitInfo, currentFocus);

					}

					if (isMouseInside()) {
						hitInfo.mouseHit = true;
					}
				}
			}

			void ScrollArea::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {

				if (visible) {
					Theme::GuiTheme* theme = gTheme;

					if (nullptr != themeOverride) {
						theme = themeOverride;
					}

					calculatePoints(vpMatRef);

					int textureSlot = 0;

					// setup shader
					shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);

					if (nullptr != theme->list.textureItem) {

						theme->list.textureItem->bind();

						shaderContainer.standardQuad->bind();
						shaderContainer.standardQuad->render();
					}

					glm::mat4 cpy = vpMatRef;
					vertBar->render(cpy, shaderContainer);
					cpy = vpMatRef;
					horzBar->render(cpy, shaderContainer);

					const ScissorInfo scinfo = gRenderEngine->pushScissorRegion(absolutePosition.x, absolutePosition.y, absoluteSize.x - SCROLLBAR_SIZE, absoluteSize.y - SCROLLBAR_SIZE);

					if (nullptr != child) {
						child->render(vpMatRef, shaderContainer);
					}

					gRenderEngine->popScissorRegion(scinfo);

				}
			}

		}
	}
}