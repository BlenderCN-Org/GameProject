#ifndef GUI_ITEM_HPP
#define GUI_ITEM_HPP

/// Internal Includes
#include "ShaderContainer.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {

		enum class GuiAnchor {
			CENTER,
			TOP,
			BOTTOM,
			LEFT,
			RIGHT,
			TOP_LEFT,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
		};

		class GuiItem {
		public:
			GuiItem();
			virtual ~GuiItem();

			void setPosition(int x, int y);
			void setSize(int w, int h);

			void setVisible(bool visibilityFlag);

			void setAnchorPoint(GuiAnchor anchor);
			virtual void update(float dt);
			virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer);

			void updateAbsoultePos(const int xOff, const int yOff, const int xSize, const int ySize);

		protected:

			void calculatePoints(glm::mat4 &positionMatrix);
			glm::vec4 clipRegion(glm::vec4 element, glm::vec4 region);

			bool posInItem(int x, int y);

			glm::vec4 positionAndSizeFromMatrix(const glm::mat4 &positionMatrix);

			glm::ivec2 position;
			glm::ivec2 size;
			glm::ivec2 absoulutePosition;

			glm::vec4 uvCoords;

			GuiAnchor anchorPoint;

			bool visible;
		};
	}
}

#endif