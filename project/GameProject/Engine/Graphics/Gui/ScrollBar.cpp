/// Internal Includes
#include "ScrollBar.hpp"
#include "../../Input/Input.hpp"
#include "../../Core/System.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			ScrollBar::ScrollBar()
				: scrollDir(ScrollBarDirection::SCROLL_VERTICAL)
				, numElements(0)
				, minElements(0)
				, selectedElement(0)
				, autoScrollLastElement(false)
				, scrollbarScreenPosition(0)
				, scrollbarThickness(10)
				, backgroundTexture(nullptr)
				, scrollbarTexture(nullptr)
				, holdingBar(false)
				, forceUpdate(false) {}

			ScrollBar::~ScrollBar() {
				backgroundTexture = nullptr;
				scrollbarTexture = nullptr;
			}

			void ScrollBar::setAutoScroll(bool autoScroll) {
				autoScrollLastElement = autoScroll;
			}

			void ScrollBar::setScrollDirection(ScrollBarDirection dir) {
				scrollDir = dir;
			}

			void ScrollBar::setNumElements(int32_t num) {
				if (num != numElements) {
					numElements = num;
					forceUpdate = true;
				}
			}
			void ScrollBar::setMinElements(int32_t num) {
				if (num != minElements) {
					minElements = num;
					forceUpdate = true;
				}
			}

			void ScrollBar::selectElement(int32_t element) {

				if (element != selectedElement) {

					int32_t lastSelectableElement = numElements - minElements;

					int minPos = 0;
					int maxPos = 0;

					if (element > lastSelectableElement) {
						element = lastSelectableElement;
					}
					if (element < 0) {
						element = 0;
					}

					float elementPos = System::percentageInRange<int32_t>(0, lastSelectableElement, element);

					float scrollThicknessPer = System::percentageInRange<int32_t>(0, numElements, minElements);

					if (scrollDir == ScrollBarDirection::SCROLL_HORIZONTAL) {

						scrollbarThickness = (int)(float(size.x) * scrollThicknessPer);

						if (scrollbarThickness < 10) {
							scrollbarThickness = 10;
						}

						minPos = absoulutePosition.x;
						maxPos = absoulutePosition.x + (size.x - scrollbarThickness);

					} else {

						scrollbarThickness = (int)(float(size.y) * scrollThicknessPer);

						if (scrollbarThickness < 10) {
							scrollbarThickness = 10;
						}

						minPos = absoulutePosition.y;
						maxPos = absoulutePosition.y + (size.y - scrollbarThickness);

					}

					scrollbarScreenPosition = int(float(maxPos - minPos) * elementPos);

					//float per = System::percentageInRange(0, maxPos - minPos, scrollbarScreenPosition);

					//selectedElement = (int32_t)(float(lastSelectableElement) * per);
					selectedElement = element;
				}
			}

			int32_t ScrollBar::getSelectedElement() const {
				return selectedElement;
			}

			void ScrollBar::setBackgroundTexture(Texture::Texture2D* texture) {
				backgroundTexture = texture;
			}

			void ScrollBar::setScrollbarTexture(Texture::Texture2D* texture) {
				scrollbarTexture = texture;
			}

			void ScrollBar::update(float dt, GuiHitInfo& hitInfo) {

				if (visible) {
					// if we cannot scroll always select first element
					if (numElements <= minElements) {
						scrollbarScreenPosition = 0;
						selectedElement = 0;
						if (scrollDir == ScrollBarDirection::SCROLL_HORIZONTAL) {
							scrollbarThickness = size.x;
						} else {
							scrollbarThickness = size.y;
						}

					} else {

						Input::Input* in = Input::Input::GetInput();

						// check for input
						if (holdingBar) { //is scrollbar being held

							if (in->releasedThisFrame(Input::KeyBindings[Input::KEYBINDS_NAME::KEYBIND_MOUSE_L_CLICK], false)) {
								holdingBar = false;
							}

							int mx = 0;
							int my = 0;
							in->getMousePos(mx, my);

							updateBarPosition(mx, my);

						} else {

							if (forceUpdate) {

								if (autoScrollLastElement) {
									int32_t p = 0x7FFFFFFF;
									updateBarPosition(p, p);
								} else {
									updateBarPosition(0, 0);
								}

								forceUpdate = false;
							}

							if (in->wasPressedThisFrame(Input::KeyBindings[Input::KEYBINDS_NAME::KEYBIND_MOUSE_L_CLICK], false)) {

								int mx = 0;
								int my = 0;
								in->getMousePos(mx, my);
								if (posInItem(mx, my)) {
									holdingBar = true;
								}
							}
						}
					}

					if (isMouseInside()) {
						hitInfo.mouseHit = true;
					}
				}
			}

			void ScrollBar::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {

				if (visible) {
					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

					calculatePoints(vpMatRef);

					int textureSlot = 0;

					shaderContainer.standardQuad->bind();

					// setup shader
					shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);
					if (backgroundTexture) {
						backgroundTexture->bind();
					}
					// render background
					shaderContainer.standardQuad->render();

					glm::vec4 scrollAreaSize = positionAndSizeFromMatrix(vpMatRef);

					if (scrollDir == ScrollBarDirection::SCROLL_HORIZONTAL) {
						vpMatRef[0].x += scrollbarScreenPosition + 1;
						vpMatRef[1].x += scrollbarScreenPosition + 1;

						vpMatRef[2].x = vpMatRef[0].x + scrollbarThickness - 2;
						vpMatRef[3].x = vpMatRef[1].x + scrollbarThickness - 2;

						vpMatRef[0].y++;
						vpMatRef[3].y++;

						vpMatRef[1].y -= 2;
						vpMatRef[2].y -= 2;

					} else {
						vpMatRef[0].y += scrollbarScreenPosition + 1;
						vpMatRef[3].y += scrollbarScreenPosition + 1;

						vpMatRef[1].y = vpMatRef[0].y + scrollbarThickness - 2;
						vpMatRef[2].y = vpMatRef[3].y + scrollbarThickness - 2;

						vpMatRef[0].x++;
						vpMatRef[1].x++;

						vpMatRef[2].x -= 2;
						vpMatRef[3].x -= 2;
					}

					shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);
					if (scrollbarTexture) {
						scrollbarTexture->bind();
					}

					// render bar
					shaderContainer.standardQuad->render();

				}
			}

			void ScrollBar::updateBarPosition(int mx, int my) {

				int32_t lastSelectableElement = numElements - minElements;

				int minPos = 0;
				int maxPos = 0;

				float scrollThicknessPer = System::percentageInRange<int32_t>(0, numElements, minElements);

				if (scrollDir == ScrollBarDirection::SCROLL_HORIZONTAL) {
					scrollbarThickness = (int)(float(size.x) * scrollThicknessPer);

					if (scrollbarThickness < 10) {
						scrollbarThickness = 10;
					}

					minPos = absoulutePosition.x;
					maxPos = absoulutePosition.x + (size.x - scrollbarThickness);

					mx -= (absoulutePosition.x + (scrollbarThickness / 2));

					if (mx < 0) {
						mx = 0;
					}

					if (mx > (size.x - scrollbarThickness)) {
						mx = (size.x - scrollbarThickness);
					}

					scrollbarScreenPosition = mx;

				} else {
					scrollbarThickness = (int)(float(size.y) * scrollThicknessPer);

					if (scrollbarThickness < 10) {
						scrollbarThickness = 10;
					}

					minPos = absoulutePosition.y;
					maxPos = absoulutePosition.y + (size.y - scrollbarThickness);

					my -= (absoulutePosition.y + (scrollbarThickness / 2));

					if (my < 0) {
						my = 0;
					}

					if (my > (size.y - scrollbarThickness)) {
						my = (size.y - scrollbarThickness);
					}

					scrollbarScreenPosition = my;
				}
				float per = System::percentageInRange(0, maxPos - minPos, scrollbarScreenPosition);

				selectedElement = (int32_t)(float(lastSelectableElement) * per);

			}
		}
	}
}