#include "GuiItem.hpp"
#include "../../../Core/Input/Input.hpp"

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

void GuiItem::calculatePoints(glm::mat4 & positionMatrix) {

	int x = 0;
	int y = 0;

	Input::getInput()->getWindowSize(x, y);

	if (anchorPoint == GuiAnchor::CENTER) {
		x /= 2;
		y /= 2;

		glm::vec4 p0 = glm::vec4(x - size.x, y - size.y, 0, 0);
		glm::vec4 p1 = glm::vec4(x - size.x, y + size.y, 0, 0);
		glm::vec4 p2 = glm::vec4(x + size.x, y + size.y, 0, 0);
		glm::vec4 p3 = glm::vec4(x + size.x, y - size.y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	} else if (anchorPoint == GuiAnchor::TOP) {
		x /= 2;
		y = 0;

		glm::vec4 p0 = glm::vec4(x - size.x, y, 0, 0);
		glm::vec4 p1 = glm::vec4(x - size.x, y + 2 * size.y, 0, 0);
		glm::vec4 p2 = glm::vec4(x + size.x, y + 2 * size.y, 0, 0);
		glm::vec4 p3 = glm::vec4(x + size.x, y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	} else if (anchorPoint == GuiAnchor::BOTTOM) {
		x /= 2;

		glm::vec4 p0 = glm::vec4(x - size.x, y - 2 * size.y, 0, 0);
		glm::vec4 p1 = glm::vec4(x - size.x, y, 0, 0);
		glm::vec4 p2 = glm::vec4(x + size.x, y, 0, 0);
		glm::vec4 p3 = glm::vec4(x + size.x, y - 2 * size.y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	} else if (anchorPoint == GuiAnchor::LEFT) {
		x = 0;
		y /= 2;

		glm::vec4 p0 = glm::vec4(x, y - size.y, 0, 0);
		glm::vec4 p1 = glm::vec4(x, y + size.y, 0, 0);
		glm::vec4 p2 = glm::vec4(x + 2 * size.x, y + size.y, 0, 0);
		glm::vec4 p3 = glm::vec4(x + 2 * size.x, y - size.y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	} else if (anchorPoint == GuiAnchor::RIGHT) {
		y /= 2;

		glm::vec4 p0 = glm::vec4(x - 2 * size.x, y - size.y, 0, 0);
		glm::vec4 p1 = glm::vec4(x - 2 * size.x, y + size.y, 0, 0);
		glm::vec4 p2 = glm::vec4(x, y + size.y, 0, 0);
		glm::vec4 p3 = glm::vec4(x, y - size.y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	} else if (anchorPoint == GuiAnchor::TOP_LEFT) {
		x = 0;
		y = 0;

		glm::vec4 p0 = glm::vec4(x, y, 0, 0);
		glm::vec4 p1 = glm::vec4(x, y + 2 * size.y, 0, 0);
		glm::vec4 p2 = glm::vec4(x + 2 * size.x, y + 2 * size.y, 0, 0);
		glm::vec4 p3 = glm::vec4(x + 2 * size.x, y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	} else if (anchorPoint == GuiAnchor::TOP_RIGHT) {
		y = 0;

		glm::vec4 p0 = glm::vec4(x - 2 * size.x, y, 0, 0);
		glm::vec4 p1 = glm::vec4(x - 2 * size.x, y + 2 * size.y, 0, 0);
		glm::vec4 p2 = glm::vec4(x, y + 2 * size.y, 0, 0);
		glm::vec4 p3 = glm::vec4(x, y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	} else if (anchorPoint == GuiAnchor::BOTTOM_RIGHT) {

		glm::vec4 p0 = glm::vec4(x - 2 * size.x, y - 2 * size.y, 0, 0);
		glm::vec4 p1 = glm::vec4(x - 2 * size.x, y, 0, 0);
		glm::vec4 p2 = glm::vec4(x, y, 0, 0);
		glm::vec4 p3 = glm::vec4(x, y - 2 * size.y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	} else if (anchorPoint == GuiAnchor::BOTTOM_LEFT) {
		x = 0;

		glm::vec4 p0 = glm::vec4(x, y - 2 * size.y, 0, 0);
		glm::vec4 p1 = glm::vec4(x, y, 0, 0);
		glm::vec4 p2 = glm::vec4(x + 2 * size.x, y, 0, 0);
		glm::vec4 p3 = glm::vec4(x + 2 * size.x, y - 2 * size.y, 0, 0);

		positionMatrix[0] = p0;
		positionMatrix[1] = p1;
		positionMatrix[2] = p2;
		positionMatrix[3] = p3;
	}
}
