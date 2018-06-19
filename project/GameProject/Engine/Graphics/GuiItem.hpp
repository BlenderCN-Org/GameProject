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

		struct GuiHitInfo {
			bool mouseHit;
			int zIndex;
			int xPos;
			int yPos;
		};

		class GuiItem {
		public:
			GuiItem();
			virtual ~GuiItem();

			int getZIndex() const;

			bool isMouseInside() const;

			void setAnchorPoint(GuiAnchor anchor);
			void setPosition(int x, int y);
			void setSize(int w, int h);

			void setVisible(bool visibilityFlag);
			void setZIndex(int zIndex);

			void updateAbsoultePos(const int xOff, const int yOff, const int xSize, const int ySize);

			virtual void update(float dt, GuiHitInfo& hitInfo);
			virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer);


		protected:

			void calculatePoints(glm::mat4 &positionMatrix);
			glm::vec4 clipRegion(glm::vec4 element, glm::vec4 region);

			bool posInItem(int x, int y) const;

			glm::vec4 positionAndSizeFromMatrix(const glm::mat4 &positionMatrix);

			int zIndex;

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