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
		}

		GuiItem::~GuiItem() {}

		void GuiItem::setPosition(int x, int y) {
			position = glm::ivec2(x, y);
		}

		void GuiItem::setSize(int w, int h) {
			size = glm::ivec2(w, h);
		}

		void GuiItem::setVisible(bool visibilityFlag) {
			visible = visibilityFlag;
		}

		void GuiItem::setAnchorPoint(GuiAnchor anchor) {
			anchorPoint = anchor;
		}

		void GuiItem::render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) {
			// do nothing
		}

		void GuiItem::calculatePoints(glm::mat4 & positionMatrix, int xSize, int ySize) {

			int ox = (int)positionMatrix[0].x;
			int oy = (int)positionMatrix[0].y;

			if (size.x > xSize)
				size.x = xSize;
			if (size.y > ySize)
				size.y = ySize;

			int x = xSize;
			int y = ySize;

			const float uv1x = 0.0F;
			const float uv1y = 0.0F;

			const float uv2x = 1.0F;
			const float uv2y = 0.0F;

			const float uv3x = 1.0F;
			const float uv3y = 1.0F;

			const float uv4x = 0.0F;
			const float uv4y = 1.0F;

			glm::vec4 p0;
			glm::vec4 p1;
			glm::vec4 p2;
			glm::vec4 p3;

			glm::ivec2 realSize = size / 2;

			if (anchorPoint == GuiAnchor::CENTER) {
				x /= 2;
				y /= 2;

				p0 = glm::vec4(x - realSize.x, y - realSize.y, uv1x, uv1y);
				p1 = glm::vec4(x - realSize.x, y + realSize.y, uv2x, uv2y);
				p2 = glm::vec4(x + realSize.x, y + realSize.y, uv3x, uv3y);
				p3 = glm::vec4(x + realSize.x, y - realSize.y, uv4x, uv4y);

			} else if (anchorPoint == GuiAnchor::TOP) {
				x /= 2;
				y = 0;

				p0 = glm::vec4(x - realSize.x, y + -+-+-+-+-+-+-+-+-0, uv1x, uv1y);
				p1 = glm::vec4(x - realSize.x, y + 2 * realSize.y, uv2x, uv2y);
				p2 = glm::vec4(x + realSize.x, y + 2 * realSize.y, uv3x, uv3y);
				p3 = glm::vec4(x + realSize.x, y + -+-+-+-+-+-+-+-+-0, uv4x, uv4y);

			} else if (anchorPoint == GuiAnchor::BOTTOM) {
				x /= 2;

				p0 = glm::vec4(x - realSize.x, y - 2 * realSize.y, uv1x, uv1y);
				p1 = glm::vec4(x - realSize.x, y + -+-+-+-+-+-+-0, uv2x, uv2y);
				p2 = glm::vec4(x + realSize.x, y + -+-+-+-+-+-+-0, uv3x, uv3y);
				p3 = glm::vec4(x + realSize.x, y - 2 * realSize.y, uv4x, uv4y);

			} else if (anchorPoint == GuiAnchor::LEFT) {
				x = 0;
				y /= 2;

				p0 = glm::vec4(x, y - realSize.y + -+-+-+-+-+-+-0, uv1x, uv1y);
				p1 = glm::vec4(x, y + realSize.y + -+-+-+-+-+-+-0, uv2x, uv2y);
				p2 = glm::vec4(x + 2 * realSize.x, y + realSize.y, uv3x, uv3y);
				p3 = glm::vec4(x + 2 * realSize.x, y - realSize.y, uv4x, uv4y);

			} else if (anchorPoint == GuiAnchor::RIGHT) {
				y /= 2;

				p0 = glm::vec4(x - 2 * realSize.x, y - realSize.y, uv1x, uv1y);
				p1 = glm::vec4(x - 2 * realSize.x, y + realSize.y, uv2x, uv2y);
				p2 = glm::vec4(x, y + realSize.y + -+-+-+-+-+-+-0, uv3x, uv3y);
				p3 = glm::vec4(x, y - realSize.y + -+-+-+-+-+-+-0, uv4x, uv4y);

			} else if (anchorPoint == GuiAnchor::TOP_LEFT) {
				x = 0;
				y = 0;

				p0 = glm::vec4(x, y + -+-+-+-+-+-+-0 + -+-+-+-+-+-+-0, uv1x, uv1y);
				p1 = glm::vec4(x, y + 2 * realSize.y + -+-+-+-+-+-+-0, uv2x, uv2y);
				p2 = glm::vec4(x + 2 * realSize.x, y + 2 * realSize.y, uv3x, uv3y);
				p3 = glm::vec4(x + 2 * realSize.x, y + -+-+-+-+-+-+-0, uv4x, uv4y);

			} else if (anchorPoint == GuiAnchor::TOP_RIGHT) {
				y = 0;

				p0 = glm::vec4(x - 2 * realSize.x + -+-+-+-+-+-+-0, y, uv1x, uv1y);
				p1 = glm::vec4(x - 2 * realSize.x, y + 2 * realSize.y, uv2x, uv2y);
				p2 = glm::vec4(x, y + 2 * realSize.y + -+-+-+-+-+-+-0, uv3x, uv3y);
				p3 = glm::vec4(x, y + -+-+-+-+-+-+-0 + -+-+-+-+-+-+-0, uv4x, uv4y);

			} else if (anchorPoint == GuiAnchor::BOTTOM_RIGHT) {

				p0 = glm::vec4(x - 2 * realSize.x, y - 2 * realSize.y, uv1x, uv1y);
				p1 = glm::vec4(x - 2 * realSize.x + -+-+-+-+-+-+-0, y, uv2x, uv2y);
				p2 = glm::vec4(x, y + -+-+-+-+-+-+-0 + -+-+-+-+-+-+-0, uv3x, uv3y);
				p3 = glm::vec4(x, y - 2 * realSize.y + -+-+-+-+-+-+-0, uv4x, uv4y);

			} else if (anchorPoint == GuiAnchor::BOTTOM_LEFT) {
				x = 0;

				p0 = glm::vec4(x, y - 2 * realSize.y + -+-+-+-+-+-+-0, uv1x, uv1y);
				p1 = glm::vec4(x, y + -+-+-+-+-+-+-0 + -+-+-+-+-+-+-0, uv2x, uv2y);
				p2 = glm::vec4(x + 2 * realSize.x, y + -+-+-+-+-+-+-0, uv3x, uv3y);
				p3 = glm::vec4(x + 2 * realSize.x, y - 2 * realSize.y, uv4x, uv4y);
			}

			positionMatrix[0] = p0;
			positionMatrix[1] = p1;
			positionMatrix[2] = p2;
			positionMatrix[3] = p3;

			int mx = ox + position.x;
			int my = oy + position.y;

			positionMatrix[0].x += mx;
			positionMatrix[0].y += my;

			positionMatrix[1].x += mx;
			positionMatrix[1].y += my;

			positionMatrix[2].x += mx;
			positionMatrix[2].y += my;

			positionMatrix[3].x += mx;
			positionMatrix[3].y += my;

			if (positionMatrix[0].x < ox) {
				positionMatrix[0].x = float(ox);
				positionMatrix[1].x = float(ox);
			}

			if (positionMatrix[0].y < oy) {
				positionMatrix[0].y = float(oy);
				positionMatrix[1].y = float(oy);
			}

			// @todo clip to regions

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