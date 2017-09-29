#include "GuiItem.hpp"

GuiItem::GuiItem() {
	position = glm::ivec2(0);
	size = glm::ivec2(0);
	anchorPoint = GuiAnchor::CENTER;
}

GuiItem::~GuiItem() {}

void GuiItem::setPosition(int x, int y) {
	position = glm::ivec2(x, y);
}

void GuiItem::setSize(int w, int h) {
	size = glm::ivec2(w, h);
}

void GuiItem::setAnchorPoint(GuiAnchor anchor) {
	anchorPoint = anchor;
}

void GuiItem::render(glm::mat4 &vpMatRef) {
	// do nothing
}
