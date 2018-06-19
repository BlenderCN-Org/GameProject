/// Internal Includes
#include "GuiItem.hpp"

#include "../Input/Input.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {


		GuiItem::GuiItem() {
			position = glm::ivec2(0);
			size = glm::ivec2(0);
			anchorPoint = GuiAnchor::CENTER;
			uvCoords = glm::vec4(0.0F, 0.0F, 1.0F, 1.0F);
			zIndex = 0;
		}

		GuiItem::~GuiItem() {}

		bool GuiItem::isMouseInside() const {

			bool inside = false;
			if (visible) {
				Input::Input* in = Input::Input::GetInput();
				int x, y;
				in->getMousePos(x, y);
				inside = posInItem(x, y);
			}
			return inside;
		}

		int GuiItem::getZIndex() const {
			return zIndex;
		}

		void GuiItem::setAnchorPoint(GuiAnchor anchor) {
			anchorPoint = anchor;
		}

		void GuiItem::setPosition(int x, int y) {
			position = glm::ivec2(x, y);
		}

		void GuiItem::setSize(int w, int h) {
			size = glm::ivec2(w, h);
		}

		void GuiItem::setVisible(bool visibilityFlag) {
			visible = visibilityFlag;
		}

		void GuiItem::setZIndex(int _zIndex) {
			zIndex = _zIndex;
		}

		void GuiItem::updateAbsoultePos(const int xOff, const int yOff, const int xSize, const int ySize) {

			const int ox = xOff;
			const int oy = yOff;

			if (size.x > xSize)
				size.x = xSize;
			if (size.y > ySize)
				size.y = ySize;

			int x = xSize;
			int y = ySize;

			glm::ivec2 realSize = size / 2;

			int aX;
			int aY;

			if (anchorPoint == GuiAnchor::CENTER) {
				x /= 2;
				y /= 2;

				aX = x - realSize.x;
				aY = y - realSize.y;

			} else if (anchorPoint == GuiAnchor::TOP) {
				x /= 2;
				y = 0;

				aX = x - realSize.x;
				aY = y;

			} else if (anchorPoint == GuiAnchor::BOTTOM) {
				x /= 2;

				aX = x - realSize.x;
				aY = y - 2 * realSize.y;

			} else if (anchorPoint == GuiAnchor::LEFT) {
				x = 0;
				y /= 2;

				aX = x;
				aY = y - realSize.y;

			} else if (anchorPoint == GuiAnchor::RIGHT) {
				y /= 2;

				aX = x - 2 * realSize.x;
				aY = y - realSize.y;

			} else if (anchorPoint == GuiAnchor::TOP_LEFT) {
				x = 0;
				y = 0;

				aX = x;
				aY = y;

			} else if (anchorPoint == GuiAnchor::TOP_RIGHT) {
				y = 0;

				aX = x - 2 * realSize.x;
				aY = y;

			} else if (anchorPoint == GuiAnchor::BOTTOM_RIGHT) {

				aX = x - 2 * realSize.x;
				aY = y - 2 * realSize.y;

			} else if (anchorPoint == GuiAnchor::BOTTOM_LEFT) {
				x = 0;

				aX = x - 2 * realSize.x;
				aY = y;
			}

			int mx = ox + position.x;
			int my = oy + position.y;

			aX += mx;
			aY += my;

			if (aX < ox) {
				aX = ox;
			}

			if (aY < oy) {
				aY = oy;
			}

			absoulutePosition.x = aX;
			absoulutePosition.y = aY;

		}

		void GuiItem::update(float dt, GuiHitInfo& hitInfo) {
			// do nothing
			if (visible) {
				if (isMouseInside()) {
					hitInfo.mouseHit = true;
				}
			}
		}

		void GuiItem::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
			// do nothing
		}

		void GuiItem::calculatePoints(glm::mat4 &positionMatrix) {

			const float uv1x = uvCoords[0];
			const float uv1y = uvCoords[1];

			//const float uv2x = uvCoords[2];
			//const float uv2y = uvCoords[1];

			const float uv2x = uvCoords[0];
			const float uv2y = uvCoords[3];

			const float uv3x = uvCoords[2];
			const float uv3y = uvCoords[3];

			//const float uv4x = uvCoords[0];
			//const float uv4y = uvCoords[3];

			const float uv4x = uvCoords[2];
			const float uv4y = uvCoords[1];

			const float x = float(absoulutePosition.x);
			const float y = float(absoulutePosition.y);

			glm::vec4 p0 = glm::vec4(x, y, uv1x, uv1y);
			glm::vec4 p1 = glm::vec4(x, y + size.y, uv2x, uv2y);
			glm::vec4 p2 = glm::vec4(x + size.x, y + size.y, uv3x, uv3y);
			glm::vec4 p3 = glm::vec4(x + size.x, y, uv4x, uv4y);

			positionMatrix[0] = p0;
			positionMatrix[1] = p1;
			positionMatrix[2] = p2;
			positionMatrix[3] = p3;

		}

		glm::vec4 GuiItem::clipRegion(glm::vec4 element, glm::vec4 region) {

			glm::vec4 newPos = element;

			if (element[0] < region[0])
				newPos[0] = region[0];
			if (element[1] < region[1])
				newPos[1] = region[1];
			if (element[2] > region[2])
				newPos[2] = region[2];
			if (element[3] > region[3])
				newPos[3] = region[3];

			return newPos;
		}

		bool GuiItem::posInItem(int x, int y) const {

			x -= absoulutePosition.x;
			y -= absoulutePosition.y;

			if ((x > 0 && x < size.x) &&
				(y > 0 && y < size.y)) {
				return true;
			}
			return false;
		}

		glm::vec4 GuiItem::positionAndSizeFromMatrix(const glm::mat4 &positionMatrix) {

			glm::vec4 p0 = positionMatrix[0];
			glm::vec4 p2 = positionMatrix[2];

			float x = p0.x;
			float y = p0.y;
			float w = p2.x - p0.x;
			float h = p2.y - p0.y;

			return glm::vec4(x, y, w, h);
		}
	}
}