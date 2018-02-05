
/// Internal Includes
#include "GuiWindow.hpp"
#include "../Graphics.hpp"
#include "../../Input/Input.hpp"

/// External Includes

/// Std Includes

/// Constants

const int GUI_WINDOW_TITLEBAR_HEIGHT = 30;

namespace Engine {
	namespace Graphics {
		namespace Gui {

			GuiWindow::GuiWindow() {

				background = new Texture::Texture2D();
				titleBar = new Texture::Texture2D();
				closeButton = new Button();

				buttonTexture = new Texture::Texture2D();
				buttonHover = new Texture::Texture2D();
				buttonPress = new Texture::Texture2D();

				itemPanel = new Panel();

				itemPanel->setVisible(true);
				itemPanel->setAnchorPoint(GuiAnchor::TOP);
				itemPanel->setPosition(0, GUI_WINDOW_TITLEBAR_HEIGHT);
				itemPanel->setTexture(background);

				background->singleColor(0.5F, 0.5F, 0.5F, 1.0F);
				titleBar->singleColor(0.2F, 0.2F, 0.2F, 1.0F);

				buttonTexture->singleColor(0.8F, 0.8F, 0.8F, 1.0F);
				buttonHover->singleColor(0.9F, 0.6F, 0.6F, 1.0F);
				buttonPress->singleColor(1.0F, 1.0F, 1.0F, 1.0F);

				closeButton->setVisible(true);
				closeButton->setPosition(-5, 5);

				closeButton->setSize(20, 20);
				closeButton->setAnchorPoint(GuiAnchor::TOP_RIGHT);
				closeButton->setText("X");
				closeButton->setTexture(buttonTexture);
				closeButton->setHoverTexture(buttonHover);
				closeButton->setPressTexture(buttonPress);

				subItems.push_back(closeButton);
				subItems.push_back(itemPanel);

				oldX = 0;
				oldY = 0;
				updating = false;
			}

			GuiWindow::~GuiWindow() {
				delete background;
				delete titleBar;
				delete closeButton;

				delete buttonTexture;
				delete buttonHover;
				delete buttonPress;

				delete itemPanel;
			}

			void GuiWindow::addGuiItem(GuiItem* guiItem) {
				itemPanel->addGuiItem(guiItem);
			}

			void GuiWindow::update(float dt) {
				if (visible) {

					if (!closeButton->isMouseInside()) {
						Input::Input* in = Input::Input::GetInput();

						int x = 0;
						int y = 0;

						in->getMousePos(x, y);

						int mx, my;

						mx = x;
						my = y;

						x -= absoulutePosition.x;
						y -= absoulutePosition.y;

						bool inside = false;

						if ((x > 0 && x < size.x) &&
							(y > 0 && y < GUI_WINDOW_TITLEBAR_HEIGHT)) {
							inside = true;
						}

						if (inside || updating) {

							if (in->isKeyBindPressed(Input::KeyBindings[Input::KEYBIND_MOUSE_L_CLICK])) {

								int px = position.x;
								int py = position.y;

								px += (mx - oldX);
								py += (my - oldY);

								setPosition(px, py);

								updating = true;

							} else {
								updating = false;
							}

							oldX = mx;
							oldY = my;
						}

					}

					itemPanel->setSize(size.x, size.y - GUI_WINDOW_TITLEBAR_HEIGHT);

					if (closeButton->wasPressed()) {
						visible = false;
					}

					setAnchorPoint(GuiAnchor::TOP_LEFT);

					std::vector<GuiItem*>::iterator it = subItems.begin();
					std::vector<GuiItem*>::iterator eit = subItems.end();

					for (it; it != eit; it++) {
						(*it)->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x, size.y);
						(*it)->update(dt);
					}
				}
			}

			void GuiWindow::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
				if (visible) {

					calculatePoints(vpMatRef);
					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

					int textureSlot = 0;

					// setup shader
					shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);

					shaderContainer.standardQuad->bind();

					background->bind();
					shaderContainer.standardQuad->render();

					gRenderEngine->setScissorTest(true);
					gRenderEngine->setScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)posAndSize.z, GUI_WINDOW_TITLEBAR_HEIGHT);

					titleBar->bind();
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

		}
	}
}