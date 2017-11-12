/// Internal Includes
#include "Label.hpp"
#include "../Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			Label::Label() {
				text.setText("Test");
			}

			Label::~Label() {}

			void Label::setText(const Engine::Core::FormattedString& str) {
				text.setText(str);
			}

			void Label::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
				if (visible) {
					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);

					calculatePoints(vpMatRef, (int)posAndSize.z, (int)posAndSize.w);

					int textureSlot = 0;

					shaderContainer.guiTextShader->useShader();
					shaderContainer.guiTextShader->bindData(shaderContainer.textVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiTextShader->bindData(shaderContainer.textTransform, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiTextShader->bindData(shaderContainer.textTexture, UniformDataType::UNI_INT, &textureSlot);

					text.render(textureSlot);

					// setup shader
					/*shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);*/


				}
			}

		}
	}
}