/// Internal Includes
#include "Button.hpp"
#include "../../Input/Input.hpp"
#include "../Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			Button::Button() {

				lbl = new Label();

				lbl->setAnchorPoint(GuiAnchor::CENTER);
				lbl->setVisible(true);
				lbl->setPosition(0, 0);
				lbl->setSize(190, 50);
			}

			Button::~Button() {

				delete lbl;
			}

			bool Button::wasPressed() {
				bool click = clicked;
				clicked = false;
				return click;
			}

			void Button::setTexture(Texture::Texture2D* texture) {
				tex = texture;
			}

			void Button::setHoverTexture(Texture::Texture2D* texture) {
				hovTex = texture;
			}

			void Button::setPressTexture(Texture::Texture2D* texture) {
				pressTex = texture;
			}

			void Button::setText(const Engine::Core::FormattedString& str) {
				lbl->setText(str);
				lbl->setSize(lbl->calcTextWidth(), size.y / 2);
			}

			void Button::update(float dt) {

				lbl->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x, size.y);
				lbl->update(dt);

				Input::Input* in = Input::Input::GetInput();

				if (hovering) {

					int mx = 0;
					int my = 0;
					in->getMousePos(mx, my);
					if (!posInItem(mx, my)) {
						hovering = false;
					}

					if (in->wasPressedThisFrame(Input::KeyBindings[Input::KEYBIND_MOUSE_L_CLICK])) {
						pressing = true;
					}

				} else {

					int mx = 0;
					int my = 0;
					in->getMousePos(mx, my);
					if (posInItem(mx, my)) {
						hovering = true;
					}

				}

				if (in->releasedThisFrame(Input::KeyBindings[Input::KEYBIND_MOUSE_L_CLICK])) {

					if (pressing && hovering) {
						clicked = true;
						pressing = false;
					} else {
						pressing = false;
					}

				}

			}

			void Button::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {

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

					if (hovering) {
						if (pressing) {
							if (pressTex) {
								pressTex->bind();
							}
						} else {
							if (hovTex) {
								hovTex->bind();
							}
						}
					}

					shaderContainer.standardQuad->bind();
					shaderContainer.standardQuad->render();

					posAndSize = positionAndSizeFromMatrix(vpMatRef);
					// render all subitems
					glm::mat4 cpy = vpMatRef;
					gRenderEngine->setScissorTest(true);
					gRenderEngine->setScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)posAndSize.z, (int)posAndSize.w);
					lbl->render(cpy, shaderContainer);
					gRenderEngine->setScissorTest(false);

				}

			}

		}
	}
}