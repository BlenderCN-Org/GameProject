/// Internal Includes
#include "Button.hpp"
#include "../Graphics.hpp"
#include "../../Input/Input.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			Button::Button(GuiInfo& info)
				: GuiItem(info)
				, lbl(nullptr)
				, hovering(false)
				, pressing(false)
				, clicked(false)
				, enabled(true)
			{

				lbl = new Label(info);

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

			void Button::setEnabled(bool enable) {
				enabled = enable;
			}

			bool Button::getEnabled() const {
				return enabled;
			}
			
			void Button::setIcon(Engine::Graphics::Texture::Texture2D* ico) {
				lbl->setIcon(ico);
				lbl->setSize(lbl->calcTextWidth(), lbl->calcTextHeight() + 3);
			}

			void Button::setText(const Engine::Core::FormattedString& str) {
				lbl->setText(str);
				lbl->setSize(lbl->calcTextWidth(), lbl->calcTextHeight() + 3);
			}

			bool Button::isFocusable() const {
				return true;
			}

			bool Button::hasFocusableItems() const {
				return false;
			}

			void Button::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {

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
				if (true == hitInfo.mouseHit) {
					hovering = false;
				}

				if (in->releasedThisFrame(Input::KeyBindings[Input::KEYBIND_MOUSE_L_CLICK])) {

					if (pressing && hovering) {
						clicked = true;
						pressing = false;
					} else {
						pressing = false;
					}

				}

				if (isMouseInside()) {
					hitInfo.mouseHit = true;
				}

				lbl->updateAbsoultePos(absolutePosition.x, absolutePosition.y, size.x, size.y);
				lbl->update(dt, hitInfo, currentFocus);

			}

			void Button::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {

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
					
					Texture::Texture2D* texture = theme->button.textureNormal;
					
					if (hovering && enabled) {
						if (pressing) {
							texture = theme->button.texturePressing;
						} else {
							texture = theme->button.textureHovering;
						}
					}

					// only render if there is an actual texture
					if(texture != nullptr)
					{
						texture->bind();
						shaderContainer.standardQuad->bind();
						shaderContainer.standardQuad->render();
					}
					
					posAndSize = positionAndSizeFromMatrix(vpMatRef);
					// render all subitems
					glm::mat4 cpy = vpMatRef;
					const ScissorInfo scinfo = gRenderEngine->pushScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)posAndSize.z, (int)posAndSize.w);
					lbl->render(cpy, shaderContainer);
					gRenderEngine->popScissorRegion(scinfo);

				}

			}

		}
	}
}