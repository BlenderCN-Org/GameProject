/// Internal Includes
#include "TextArea.hpp"
#include "../Graphics.hpp"
#include "../../Input/Input.hpp"

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
				lineNumbers = true;
				allowEdit = false;
				editCursorChar = 0;
				editCursorLine = 0;
				renderCursor = false;


				cursorTexture = new Texture::Texture2D();
				float cursorColor = 0.8F;
				cursorTexture->singleColor(cursorColor, cursorColor, cursorColor, 1.0F);

			}

			TextArea::~TextArea() {
				delete verticalScroll;
				delete scrollBg;
				delete scrollBar;
				delete cursorTexture;
			}

			void TextArea::setEditMode(bool edit) {
				allowEdit = edit;
				if (!edit) {
					renderCursor = false;
				}
			}

			void TextArea::showLineNumbers(bool show) {
				lineNumbers = show;
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

				if (allowEdit) {
					if (isMouseInside()) {

						Input::Input* in = Input::Input::GetInput();

						if (in->characterRecieved) {

							char c = (char)in->characterThisFrame;

							textData[editCursorLine].insertAt(editCursorChar++, c);

						}

						if (in->returnPressed) {

							Engine::Core::FormattedString fString = textData[editCursorLine];
							Engine::Core::FormattedString str2;

							if (editCursorChar) {
								str2 = fString.subString(0, editCursorChar);
								textData[editCursorLine] = str2;
							} else {
								textData[editCursorLine] = Engine::Core::FormattedString("");
							}

							str2 = fString.subString(editCursorChar, 0);

							textData.insert(textData.begin() + editCursorLine + 1, str2);

							editCursorLine++;
							editCursorChar = 0;

						}

						if (in->backspacePressed) {

							// delete char on current line
							if (editCursorChar) {
								textData[editCursorLine].remoteAt(--editCursorChar);
							}
							// delete the line and merge upwards
							else {

								if (editCursorLine) {
									int cursor = (int)textData[editCursorLine - 1].getSize();
									textData[editCursorLine - 1] += textData[editCursorLine];
									textData.erase(textData.begin() + editCursorLine);
									editCursorLine--;
									editCursorChar = cursor;
								}
							}
						}

						if (in->wasPressedThisFrame(Input::KeyBindings[Input::KEYBIND_UP_ARROW])) {
							if (editCursorLine) {
								editCursorLine--;
							}

							int lineLength = (int)textData[editCursorLine].getSize();

							if (lineLength < editCursorChar) {
								editCursorChar = lineLength;
							}

						}

						if (in->wasPressedThisFrame(Input::KeyBindings[Input::KEYBIND_DOWN_ARROW])) {

							editCursorLine++;

							if ((int)textData.size() <= editCursorLine) {
								editCursorLine = textData.size() - 1;
							}

							int lineLength = (int)lineLength = textData[editCursorLine].getSize();

							if (lineLength < editCursorChar) {
								editCursorChar = lineLength;
							}

						}

						if (in->wasPressedThisFrame(Input::KeyBindings[Input::KEYBIND_LEFT_ARROW])) {
							if (editCursorChar) {
								editCursorChar--;
							} else {
								if (editCursorLine) {
									editCursorLine--;
						
									editCursorChar = textData[editCursorLine].getSize();
								}
							}
						}
						
						if (in->wasPressedThisFrame(Input::KeyBindings[Input::KEYBIND_RIGHT_ARROW])) {
							
							editCursorChar++;
						
							if ((int)textData[editCursorLine].getSize() < editCursorChar) {
								editCursorLine++;
								if ((int)textData.size() <= editCursorLine) {
									editCursorLine = textData.size() - 1;
									editCursorChar = textData[editCursorLine].getSize();
								}
							}
						}
					}

					if (editCursorChar) {
						Engine::Core::FormattedString tmpStr = textData[editCursorLine].subString(0, editCursorChar);
						text.setText(tmpStr);
					} else {
						text.setText("");
					}

					cursorTimer -= dt;

					if (cursorTimer < 0.0F) {
						renderCursor = !renderCursor;
						cursorTimer += 0.4F;
					}

					cursorRenderXOffset = text.getTextWidth();

				}


				const int fntSize = text.getFontSize();
				cursorHeight = fntSize;
				const int elementsPerScreen = size.y / fntSize;
				const int32_t vectorSize = (int32_t)textData.size();

				verticalScroll->setSize(15, size.y);
				verticalScroll->setMinElements(elementsPerScreen);
				verticalScroll->setNumElements(vectorSize);

				sizeYClipp = elementsPerScreen * fntSize;

				verticalScroll->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x, size.y);
				verticalScroll->update(dt);

				int32_t selectedIndex = verticalScroll->getSelectedElement();

				Engine::Core::FormattedString fString;

				std::string maxLinCnt = std::to_string(vectorSize);

				cursorRenderYOffset = cursorHeight * -1;

				for (int i = 0; i < elementsPerScreen; i++) {
					if (selectedIndex + i >= vectorSize) {
						break;
					}

					if (selectedIndex + i == editCursorLine) {
						cursorRenderYOffset = cursorHeight * i;
					}

					std::string s = std::to_string(selectedIndex + i);

					if (lineNumbers) {
						Engine::Core::FormattedString lineNumber;

						for (size_t i = 0; i < maxLinCnt.size() - s.size(); i++) {
							lineNumber += "0";
						}

						lineNumber += s.c_str();
						lineNumber += ": ";

						lineNumber.formatString(0, -1, glm::vec4(0.418924, 0.573681, 0.626665, 1.000000));

						fString += lineNumber;
					}

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

					if (renderCursor) {

						cursorTexture->bind();

						gRenderEngine->setScissorRegion(absoulutePosition.x + cursorRenderXOffset, absoulutePosition.y + cursorRenderYOffset, 1, cursorHeight);
						shaderContainer.guiElementShader->useShader();
						shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
						shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
						shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);

						shaderContainer.standardQuad->bind();
						shaderContainer.standardQuad->render();
					}

					gRenderEngine->setScissorTest(false);
				}

			}

		}
	}
}