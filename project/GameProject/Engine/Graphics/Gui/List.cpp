/// Internal Includes
#include "List.hpp"
#include "../Graphics.hpp"
#include "../../Input/Input.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			const int LIST_ITEM_SIZE = 20;

			ListItem::ListItem(GuiInfo& info) : GuiItem(info), text(info.pAssetManager) {
				setAnchorPoint(GuiAnchor::TOP_LEFT);
				isFocused = false;
				icon = nullptr;
			}

			ListItem::~ListItem() {

			}

			void ListItem::setText(Core::String t) {
				identifier = t;
				text.setText(t);
			}

			glm::ivec2 ListItem::getAbsPos() const {
				return absoulutePosition;
			}

			void ListItem::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {

				Input::Input* in = Input::Input::GetInput();

				Input::KeyBind mouseClick = Input::KeyBindings[Input::KEYBIND_MOUSE_L_CLICK];
				if (in->releasedThisFrame(mouseClick, false)) {
					if (isMouseInside()) {
						isFocused = true;
					} else {
						isFocused = false;
					}
				}
				if (isMouseInside()) {
					hitInfo.mouseHit = true;
				}
			}

			void ListItem::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
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

					if (gTheme->list.textureSelectedItem && isFocused) {
						gTheme->list.textureSelectedItem->bind();
						shaderContainer.standardQuad->bind();
						shaderContainer.standardQuad->render();
					}

					if (icon) {
						glm::ivec4 posSize = positionAndSizeFromMatrix(vpMatRef);
						icon->bind();

						unsigned int iw, ih;

						icon->getSize(iw, ih);
						posSize.x += 2;
						posSize.z = glm::min(size.y - 2, (int)iw);
						posSize.w = glm::min(size.y - 2, (int)iw);
						vpMatRef = genFromPosSize(posSize);

						shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);

						shaderContainer.standardQuad->bind();
						shaderContainer.standardQuad->render();

						posSize.x += size.y;
						//posSize.y += (size.y / 2);
						posSize.z = size.y;
						posSize.w = size.y;//text.getTextHeight();
						vpMatRef = genFromPosSize(posSize);
					}

					//std::string str = std::to_string((int)absoulutePosition.x) + ", "+ std::to_string((int)absoulutePosition.y) + ", " + std::to_string((int)size.x) + ", " + std::to_string((int)size.y);
					//text.setText(str.c_str());

					gRenderEngine->setScissorTest(true);
					gRenderEngine->setScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)posAndSize.z, (int)posAndSize.w);

					shaderContainer.guiTextShader->useShader();
					shaderContainer.guiTextShader->bindData(shaderContainer.textVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiTextShader->bindData(shaderContainer.textTransform, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiTextShader->bindData(shaderContainer.textTexture, UniformDataType::UNI_INT, &textureSlot);

					text.render(textureSlot);

					gRenderEngine->setScissorTest(false);

				}
			}

			List::List(GuiInfo& info) : GuiItem(info) {

				verticalScroll = new ScrollBar(info);
				verticalScroll->setAnchorPoint(GuiAnchor::RIGHT);
				verticalScroll->setScrollDirection(ScrollBarDirection::SCROLL_VERTICAL);
				verticalScroll->setVisible(true);
				verticalScroll->setBackgroundTexture(nullptr);
				verticalScroll->setScrollbarTexture(nullptr);
				verticalScroll->setAutoScroll(false);

				hasSearch = false;

				verticalScroll->selectElement(0);

				selectedItem = nullptr;
			}

			List::~List() {
				delete verticalScroll;
			}

			void List::setTexture(Texture::Texture2D* texture) {
				tex = texture;
			}

			void List::addListItem(ListItem* listItem) {
				listItems.push_back(listItem);
			}

			void List::removeListItem(ListItem* listItem) {

			}

			void List::clearList() {
				listItems.clear();
			}

			ListItem* List::getSelectedItem() const {
				return selectedItem;
				//ListItem* selected = nullptr;
				//std::vector<ListItem*>::const_iterator it = listItems.begin();
				//std::vector<ListItem*>::const_iterator eit = listItems.end();
				//
				//for (it; it != eit; it++) {
				//	if (true == (*it)->isFocused) {
				//		selected = *it;
				//	}
				//}
				//return selected;
			}

			void List::search(Core::String str, ListSearchFunction searchFunc) {

				seaList.clear();
				if (0 == str.getSize() || nullptr == searchFunc) {
					hasSearch = false;
					return;
				}
				hasSearch = true;
				std::vector<ListItem*>::iterator it = listItems.begin();
				std::vector<ListItem*>::iterator eit = listItems.end();

				for (it; it != eit; it++) {
					if (searchFunc(str, *it)) {
						seaList.push_back(*it);
					}
				}
			}

			void List::update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) {
				if (visible) {
					dispList.clear();

					std::vector<ListItem*> lst;

					if (hasSearch) {
						lst = seaList;
					} else {
						lst = listItems;
					}

					std::vector<ListItem*>::iterator it = lst.begin();
					std::vector<ListItem*>::iterator eit = lst.end();

					verticalScroll->setMinElements(size.y);
					verticalScroll->setNumElements(LIST_ITEM_SIZE * lst.size());

					verticalScroll->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y, size.x - 1, size.y);
					verticalScroll->setSize(15, size.y);
					verticalScroll->update(dt, hitInfo, currentFocus);

					int c = 0;

					int32_t elementsonScreen = (size.y / LIST_ITEM_SIZE);
					uint32_t firstElement = verticalScroll->getSelectedElement() / LIST_ITEM_SIZE;
					std::advance(it, firstElement);

					for (it; it != eit; it++) {
						int py = (LIST_ITEM_SIZE * (c + firstElement));// -(verticalScroll->getSelectedElement());
						(*it)->setPosition(0, py);
						(*it)->setVisible((py >= 0) ? true : true);
						(*it)->setSize(size.x, LIST_ITEM_SIZE);
						(*it)->updateAbsoultePos(absoulutePosition.x, absoulutePosition.y - verticalScroll->getSelectedElement(), size.x - 15, (LIST_ITEM_SIZE));//size.y - verticalScroll->getSelectedElement());
						(*it)->update(dt, hitInfo, currentFocus);
						if ((*it)->isFocused) {
							selectedItem = *it;
						}
						if ((*it)->getAbsPos().y > 0 && (*it)->getAbsPos().y < (size.y * 2)) {
							dispList.push_back(*it);
						}
						if (c > (elementsonScreen + 1)) {
							break;
						}
						c++;
					}
				}
			}

			void List::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
				if (visible) {

					Theme::GuiTheme* theme = gTheme;

					if (nullptr != themeOverride) {
						theme = themeOverride;
					}
					calculatePoints(vpMatRef);

					glm::vec4 posAndSize = positionAndSizeFromMatrix(vpMatRef);


					int textureSlot = 0;

					// setup shader
					shaderContainer.guiElementShader->useShader();
					shaderContainer.guiElementShader->bindData(shaderContainer.elementVpMat, UniformDataType::UNI_MATRIX4X4, &shaderContainer.orthoMatrix);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTransformMat, UniformDataType::UNI_MATRIX4X4, &vpMatRef);
					shaderContainer.guiElementShader->bindData(shaderContainer.elementTexture, UniformDataType::UNI_INT, &textureSlot);

					if (theme->list.textureBackground) {
						theme->list.textureBackground->bind();
						shaderContainer.standardQuad->bind();
						shaderContainer.standardQuad->render();
					}


					int c = 0;
					gRenderEngine->setScissorTest(true);
					for (int32_t i = 0; i < (size.y / LIST_ITEM_SIZE) + 2; i++) {

						if (i % 2 == 0) {
							int offset = -verticalScroll->getSelectedElement() % (2 * LIST_ITEM_SIZE);
							gRenderEngine->setScissorRegion((int)posAndSize.x, (int)posAndSize.y + (LIST_ITEM_SIZE * i) + offset, (int)posAndSize.z - 15, (int)LIST_ITEM_SIZE);
							if (gTheme->list.textureItem) {
								gTheme->list.textureItem->bind();
								shaderContainer.standardQuad->bind();
								shaderContainer.standardQuad->render();
							}
						}
					}

					gRenderEngine->setScissorTest(false);

					verticalScroll->render(vpMatRef, shaderContainer);

					calculatePoints(vpMatRef);

					std::vector<ListItem*>::iterator it = dispList.begin();
					std::vector<ListItem*>::iterator eit = dispList.end();

					//posAndSize = positionAndSizeFromMatrix(vpMatRef);
					// render all subitems
					for (it; it != eit; it++) {
						glm::mat4 cpy = vpMatRef;
						gRenderEngine->setScissorTest(true);
						gRenderEngine->setScissorRegion((int)posAndSize.x, (int)posAndSize.y, (int)posAndSize.z, (int)posAndSize.w);
						if (*it == selectedItem) {
							(*it)->isFocused = true;
						}
						(*it)->render(cpy, shaderContainer);
						gRenderEngine->setScissorTest(false);
					}

				}
			}
		}
	}
}