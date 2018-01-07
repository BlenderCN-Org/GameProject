/// Internal Includes
#include "TextArea.hpp"
#include "../Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <string>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			TextArea::TextArea() : verticalScroll(nullptr), scrollBg(nullptr), scrollBar(nullptr) {
				text.setText("Test");

				scrollBg = new Texture::Texture2D();
				scrollBar = new Texture::Texture2D();

				scrollBg->singleColor(0.7F, 0.7F, 0.7F, 1.0F);
				scrollBar->singleColor(0.3F, 0.3F, 0.3F, 1.0F);

				verticalScroll = new ScrollBar();
				verticalScroll->setAnchorPoint(GuiAnchor::RIGHT);
				verticalScroll->setScrollDirection(ScrollBarDirection::SCROLL_VERTICAL);
				verticalScroll->setVisible(true);
				verticalScroll->setBackgroundTexture(scrollBg);
				verticalScroll->setScrollbarTexture(scrollBar);
				verticalScroll->setAutoScroll(true);

			}

			TextArea::~TextArea() {
				delete verticalScroll;
				delete scrollBg;
				delete scrollBar;
			}

			void TextArea::setText(const Engine::Core::FormattedString& str) {
				textData.clear();

				int idx = 0;
				int oldIdx = 0;
				while (true) {
					idx = str.indexAt('\n', idx);
					if (idx != -1) {
						int b = 0;
					}
					Engine::Core::FormattedString str2 = str.subString((size_t)oldIdx, (size_t)idx + 1);
					textData.push_back(str2);

					oldIdx = idx;
					if (idx == -1) {
						break;
					} else {
						idx++;
					}
				}
			}

			void TextArea::addText(const Engine::Core::FormattedString& str) {
				//textData += str;

				int idx = 0;
				int oldIdx = 0;
				while (true) {
					idx = str.indexAt('\n', idx);

					Engine::Core::FormattedString str2 = str.subString((size_t)oldIdx, (size_t)idx + 1);

					textData.push_back(str2);

					oldIdx = idx;
					if (idx == -1) {
						break;
					} else {
						idx++;
					}
				}

			}

			void TextArea::update(float dt) {

				const int fntSize = text.getFontSize();
				const int elementsPerScreen = size.y / fntSize;
				const size_t vectorSize = textData.size();

				verticalScroll->setSize(15, size.y);
				verticalScroll->setMinElements(elementsPerScreen);
				verticalScroll->setNumElements(vectorSize);

				sizeYClipp = elementsPerScreen * fntSize;

				verticalScroll->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x, size.y);
				verticalScroll->update(dt);

				size_t selectedIndex = verticalScroll->getSelectedElement();

				Engine::Core::FormattedString fString;

				std::string maxLinCnt = std::to_string(vectorSize);

				for (int i = 0; i < elementsPerScreen; i++) {
					if (selectedIndex + i >= vectorSize) {
						break;
					}

					std::string s = std::to_string(selectedIndex + i);
					Engine::Core::FormattedString lineNumber;

					for (size_t i = 0; i < maxLinCnt.size() - s.size() ; i++) {
						lineNumber += "0";
					}

					lineNumber += s.c_str();
					lineNumber += ": ";

					lineNumber.formatString(0, -1, glm::vec4(0.418924, 0.573681, 0.626665, 1.000000));

					fString += lineNumber;
					fString += textData[selectedIndex + i];
					fString += "\n";
				}

				text.setText(fString);

			}

			void TextArea::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {

				if (visible) {

					verticalScroll->render(vpMatRef, shaderContainer);

					calculatePoints(vpMatRef);

					int textureSlot = 0;

					shaderContainer.guiTextShader->useShader();
					shaderContainer.guiTextShader->bindData(shaderContainer.textVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiTextShader->bindData(shaderContainer.textTransform, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiTextShader->bindData(shaderContainer.textTexture, UniformDataType::UNI_INT, &textureSlot);

					gRenderEngine->setScissorRegion(absoulutePosition.x, absoulutePosition.y, size.x - 15, sizeYClipp);
					gRenderEngine->setScissorTest(true);
					text.render(textureSlot);
					gRenderEngine->setScissorTest(false);
				}

			}

			/*const int TextArea::newLinesInString(const Engine::Core::FormattedString& str) const {

				int nrNewLines = 0;

				size_t strLen = str.getSize();
				Engine::Core::FormattedChar* chrArr;

				size_t count = 0U;

				while (count < strLen) {

					if (chrArr[count] == '\n') {

					}
					count++;
				}

				return nrNewLines;
			}*/

		}
	}
}