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


			if (anchorPoint == GuiAnchor::CENTER) {
				x /= 2;
				y /= 2;

				glm::vec4 p0 = glm::vec4(x - size.x, y - size.y, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x - size.x, y + size.y, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x + size.x, y + size.y, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x + size.x, y - size.y, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			} else if (anchorPoint == GuiAnchor::TOP) {
				x /= 2;
				y = 0;

				glm::vec4 p0 = glm::vec4(x - size.x, y + -+-+-+-+-0, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x - size.x, y + 2 * size.y, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x + size.x, y + 2 * size.y, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x + size.x, y + -+-+-+-+-0, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			} else if (anchorPoint == GuiAnchor::BOTTOM) {
				x /= 2;

				glm::vec4 p0 = glm::vec4(x - size.x, y - 2 * size.y, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x - size.x, y + -+-+-+-+-0, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x + size.x, y + -+-+-+-+-0, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x + size.x, y - 2 * size.y, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			} else if (anchorPoint == GuiAnchor::LEFT) {
				x = 0;
				y /= 2;

				glm::vec4 p0 = glm::vec4(x, y - size.y + -+-+-+-+-0, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x, y + size.y + -+-+-+-+-0, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x + 2 * size.x, y + size.y, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x + 2 * size.x, y - size.y, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			} else if (anchorPoint == GuiAnchor::RIGHT) {
				y /= 2;

				glm::vec4 p0 = glm::vec4(x - 2 * size.x, y - size.y, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x - 2 * size.x, y + size.y, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x, y + size.y + -+-+-+-+-0, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x, y - size.y + -+-+-+-+-0, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			} else if (anchorPoint == GuiAnchor::TOP_LEFT) {
				x = 0;
				y = 0;

				glm::vec4 p0 = glm::vec4(x, y + -+-+-+-+-0 + -+-+-+-+-0, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x, y + 2 * size.y + -+-+-+-+-0, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x + 2 * size.x, y + 2 * size.y, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x + 2 * size.x, y + -+-+-+-+-0, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			} else if (anchorPoint == GuiAnchor::TOP_RIGHT) {
				y = 0;

				glm::vec4 p0 = glm::vec4(x - 2 * size.x + -+-+-+-+-0, y, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x - 2 * size.x, y + 2 * size.y, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x, y + 2 * size.y + -+-+-+-+-0, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x, y + -+-+-+-+-0 + -+-+-+-+-0, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			} else if (anchorPoint == GuiAnchor::BOTTOM_RIGHT) {

				glm::vec4 p0 = glm::vec4(x - 2 * size.x, y - 2 * size.y, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x - 2 * size.x + -+-+-+-+-0, y, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x, y + -+-+-+-+-0 + -+-+-+-+-0, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x, y - 2 * size.y + -+-+-+-+-0, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			} else if (anchorPoint == GuiAnchor::BOTTOM_LEFT) {
				x = 0;

				glm::vec4 p0 = glm::vec4(x, y - 2 * size.y + -+-+-+-+-0, uv1x, uv1y);
				glm::vec4 p1 = glm::vec4(x, y + -+-+-+-+-0 + -+-+-+-+-0, uv2x, uv2y);
				glm::vec4 p2 = glm::vec4(x + 2 * size.x, y + -+-+-+-+-0, uv3x, uv3y);
				glm::vec4 p3 = glm::vec4(x + 2 * size.x, y - 2 * size.y, uv4x, uv4y);

				positionMatrix[0] = p0;
				positionMatrix[1] = p1;
				positionMatrix[2] = p2;
				positionMatrix[3] = p3;
			}

			positionMatrix[0].x += ox;
			positionMatrix[0].y += oy;
								   
			positionMatrix[1].x += ox;
			positionMatrix[1].y += oy;
								   
			positionMatrix[2].x += ox;
			positionMatrix[2].y += oy;
								   
			positionMatrix[3].x += ox;
			positionMatrix[3].y += oy;

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