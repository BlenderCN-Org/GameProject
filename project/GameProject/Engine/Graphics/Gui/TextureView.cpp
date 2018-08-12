/// Internal Includes
#include "TextureView.hpp"
#include "../Graphics.hpp"
#include "../../Input/Input.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

const int32_t SCROLLBAR_SIZE = 15;
const int32_t BORDER_SIZE = 2;
const int32_t SCROLLBAR_HALF_SIZE = SCROLLBAR_SIZE / 2;

namespace Engine {
	namespace Graphics {
		namespace Gui {

			TextureView::TextureView(GuiInfo& info) : GuiItem(info) {

				tex = nullptr;

				mainText = new Texture::Texture2D();
				mainText->singleColor(0.7F, 0.7F, 0.7F, 0.5F);

				scrollBg = new Texture::Texture2D();
				scrollBar = new Texture::Texture2D();

				scrollBg->singleColor(0.7F, 0.7F, 0.7F, 1.0F);
				scrollBar->singleColor(0.3F, 0.3F, 0.3F, 1.0F);

				vertBar = new ScrollBar(info);
				vertBar->setAnchorPoint(GuiAnchor::RIGHT);
				vertBar->setScrollDirection(ScrollBarDirection::SCROLL_VERTICAL);
				vertBar->setVisible(true);
				vertBar->setBackgroundTexture(scrollBg);
				vertBar->setScrollbarTexture(scrollBar);
				vertBar->setAutoScroll(false);
				vertBar->setPosition(0, -SCROLLBAR_HALF_SIZE);

				horzBar = new ScrollBar(info);
				horzBar->setAnchorPoint(GuiAnchor::BOTTOM);
				horzBar->setScrollDirection(ScrollBarDirection::SCROLL_HORIZONTAL);
				horzBar->setVisible(true);
				horzBar->setBackgroundTexture(scrollBg);
				horzBar->setScrollbarTexture(scrollBar);
				horzBar->setAutoScroll(false);
				horzBar->setPosition(-SCROLLBAR_HALF_SIZE, 0);

			}

			TextureView::~TextureView() {
				delete mainText;
				delete scrollBg;
				delete scrollBar;
				delete vertBar;
				delete horzBar;
			}

			void TextureView::setTexture(Texture::Texture2DReference* textureRef) {
				tex = textureRef;

				if (tex) {
					tex->getTextureSize(texWidth, texHeight);
				}

				horzBar->setNumElements(texWidth);
				vertBar->setNumElements(texHeight);

			}

			void TextureView::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {
				if (visible) {

					vertBar->setMinElements(size.y - SCROLLBAR_SIZE);
					horzBar->setMinElements(size.x - SCROLLBAR_SIZE);

					vertBar->setSize(SCROLLBAR_SIZE, size.y - SCROLLBAR_SIZE);
					horzBar->setSize(size.x - SCROLLBAR_SIZE, SCROLLBAR_SIZE);

					vertBar->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x, size.y);
					horzBar->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x, size.y);

					vertBar->update(dt, hitInfo, currentFocus);
					horzBar->update(dt, hitInfo, currentFocus);

					Input::Input* in = Input::Input::GetInput();
					int x = 0, y = 0;
					in->getMousePos(x, y);

					if (posInItem(x, y)) {
						if (in->isKeyBindPressed(Input::KeyBindings[Input::KEYBIND_MOUSE_WHEEL_CLICK])) {

							int32_t elementX = horzBar->getSelectedElement();
							int32_t elementY = vertBar->getSelectedElement();

							elementX -= (x - oldX);
							elementY -= (y - oldY);

							horzBar->selectElement(elementX);
							vertBar->selectElement(elementY);
						}

						oldX = x;
						oldY = y;
					}

					offsetY = vertBar->getSelectedElement();
					offsetX = horzBar->getSelectedElement();

					if (isMouseInside()) {
						hitInfo.mouseHit = true;
					}

				}
			}

			void TextureView::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {

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
					if (mainText) {
						mainText->bind();
					}

					shaderContainer.standardQuad->bind();
					shaderContainer.standardQuad->render();

					glm::mat4 cpy = vpMatRef;
					vertBar->render(cpy, shaderContainer);
					cpy = vpMatRef;
					horzBar->render(cpy, shaderContainer);

					gRenderEngine->setScissorRegion(absoulutePosition.x, absoulutePosition.y, size.x - SCROLLBAR_SIZE, size.y - SCROLLBAR_SIZE);
					gRenderEngine->setScissorTest(true);

					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

					float x, y;
					float w, h;
					x = posAndSize.x;
					y = posAndSize.y;
					w = float(texWidth);
					h = float(texHeight);

					glm::vec4 p0 = vpMatRef[0]; //glm::vec4(x, y, uv1x, uv1y);
					glm::vec4 p1 = vpMatRef[1]; //glm::vec4(x, y + size.y, uv2x, uv2y);
					glm::vec4 p2 = vpMatRef[2]; //glm::vec4(x + size.x, y + size.y, uv3x, uv3y);
					glm::vec4 p3 = vpMatRef[3]; //glm::vec4(x + size.x, y, uv4x, uv4y);

					p1.y = y + w;
					p2.y = y + w;

					p2.x = x + h;
					p3.x = x + h;

					glm::vec4 offset = glm::vec4(offsetX, offsetY, 0, 0);

					vpMatRef[0] = p0 - offset;
					vpMatRef[1] = p1 - offset;
					vpMatRef[2] = p2 - offset;
					vpMatRef[3] = p3 - offset;

					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);

					if (tex) {
						tex->bind();
					}

					shaderContainer.standardQuad->bind();
					shaderContainer.standardQuad->render();

					gRenderEngine->setScissorTest(false);


				}
			}

		}
	}
}