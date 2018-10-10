/// Internal Includes
#include "Label.hpp"
#include "../Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			Label::Label(GuiInfo info) : GuiItem(info), text(info.pAssetManager) {
				text.setText("Test");
				icon = nullptr;
			}

			Label::~Label() {}

			int Label::calcTextWidth() const {
				uint32_t s = 0;
				uint32_t t = 0;
				if (icon != nullptr) {
					icon->getSize(s, t);
				}
				s += text.getTextWidth();
				return s;
			}

			int Label::calcTextHeight() const {
				uint32_t s = 0;
				uint32_t t = 0;
				if (icon != nullptr) {
					icon->getSize(t, s);
				}
				s += text.getTextHeight();
				return s;
			}

			void Label::setIcon(Engine::Graphics::Texture::Texture2D* ico) {
				icon = ico;
			}

			void Label::setText(const Engine::Core::FormattedString& str) {
				text.setText(str);
			}

			void Label::update(float , GuiHitInfo& hitInfo, GuiItem* ) {
				if (isMouseInside()) {
					hitInfo.mouseHit = true;
				}
			}

			void Label::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
				if (visible) {
					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

					//calculatePoints(vpMatRef, (int)posAndSize.z, (int)posAndSize.w);
					//updateAbsoultePos((int)vpMatRef[0].x, (int)vpMatRef[0].y, (int)posAndSize.z, (int)posAndSize.w);
					calculatePoints(vpMatRef);

					int textureSlot = 0;

					if (icon) {
						shaderContainer.guiElementShader->useShader();
						shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
						shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
						shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);

						glm::ivec4 posSize = positionAndSizeFromMatrix(vpMatRef);
						icon->bind();

						unsigned int iw, ih;

						icon->getSize(iw, ih);
						posSize.z = glm::min(size.y - 2, (int)iw);
						posSize.w = glm::min(size.y - 2, (int)iw);
						vpMatRef = genFromPosSize(posSize);

						shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);

						shaderContainer.standardQuad->bind();
						shaderContainer.standardQuad->render();

						posSize.x += size.y;
						posSize.z = size.y;
						posSize.w = size.y;
						vpMatRef = genFromPosSize(posSize);
					}

					shaderContainer.guiTextShader->useShader();
					shaderContainer.guiTextShader->bindData(shaderContainer.textVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiTextShader->bindData(shaderContainer.textTransform, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiTextShader->bindData(shaderContainer.textTexture, UniformDataType::UNI_INT, &textureSlot);

					const ScissorInfo scinfo = gRenderEngine->pushScissorRegion(absolutePosition.x, absolutePosition.y, size.x, size.y);
					
					text.render(textureSlot);

					gRenderEngine->popScissorRegion(scinfo);

				}
			}

		}
	}
}