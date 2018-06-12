
/// Internal Includes
#include "ProgressBar.hpp"
#include "../Graphics.hpp"
#include "../../Core/System.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {
			
			ProgressBar::ProgressBar() : min(0U), max(0U), value(0U), backTex(nullptr), progTex(nullptr) {

			}

			ProgressBar::ProgressBar(uint32_t _min, uint32_t _max, uint32_t _value) : backTex(nullptr), progTex(nullptr) {
				min = _min;
				max = _max;
				value = _value;
			}

			void ProgressBar::setValue(uint32_t _value) {
				value = _value;
			}
			
			void ProgressBar::setRange(uint32_t _min, uint32_t _max) {
				min = _min;
				max = _max;
			}

			void ProgressBar::setBackgorundTexture(Texture::Texture2D* texture) {
				backTex = texture;
			}

			void ProgressBar::setProgressTexture(Texture::Texture2D* texture) {
				progTex = texture;
			}

			void ProgressBar::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {

				if (visible) {
					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

					//calculatePoints(vpMatRef, (int)posAndSize.z, (int)posAndSize.w);

					//updateAbsoultePos((int)vpMatRef[0].x, (int)vpMatRef[0].y, (int)posAndSize.z, (int)posAndSize.w);
					calculatePoints(vpMatRef);

					int textureSlot = 0;

					// setup shader
					shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);
					if (backTex) {
						backTex->bind();
					}

					shaderContainer.standardQuad->bind();
					shaderContainer.standardQuad->render();

					posAndSize = clipRegion(positionAndSizeFromMatrix(vpMatRef), posAndSize);

					float percentage = System::percentageInRange(min, max, value);

					gRenderEngine->setScissorTest(true);
					gRenderEngine->setScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)(posAndSize.z * percentage), (int)posAndSize.w);

					if (progTex) {
						progTex->bind();
					}

					shaderContainer.standardQuad->render();
					gRenderEngine->setScissorTest(false);

				}

			}
		}
	}
}