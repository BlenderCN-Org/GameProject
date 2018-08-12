/// Internal Includes
#include "Cursor.hpp"
#include "../../Input/Input.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			Cursor::Cursor(GuiInfo& info) : GuiItem(info) {}
			Cursor::~Cursor() {}

			void Cursor::setTexture(Texture::Texture2D* texture) {
				tex = texture;
			}

			void Cursor::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {

				Input::Input::GetInput()->getMousePos(position.x, position.y);

			}

			void Cursor::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
				
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

				}
			}
		}
	}
}