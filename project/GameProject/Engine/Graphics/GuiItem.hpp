#ifndef GUI_ITEM_HPP
#define GUI_ITEM_HPP

/// Internal Includes
#include "ShaderContainer.hpp"
#include "GuiTheme.hpp"

/// External Includes
#include <EngineCore/AssetHandling/IAssetManager.hpp>
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

		struct GuiInfo {
			Engine::AssetHandling::IAssetManager* pAssetManager;
		};

		class GuiItem {
		public:
			GuiItem(GuiInfo& info);
			virtual ~GuiItem();

			int getZIndex() const;

			bool isMouseInside() const;

			void setAnchorPoint(GuiAnchor anchor);
			void setPosition(int x, int y);
			void setSize(int w, int h);

			bool isVisible() const;

			void setVisible(bool visibilityFlag);
			void setZIndex(int zIndex);

			void setHitTest(bool enable);

			void updateAbsoultePos(const int xOff, const int yOff, const int xSize, const int ySize);

			virtual bool isFocusable() const;
			virtual bool hasFocusableItems() const;



			virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus);
			virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer);

			void setThemeOverride(Theme::GuiTheme* theme);

		protected:

			void calculatePoints(glm::mat4 &positionMatrix);
			glm::mat4 genFromPosSize(glm::vec4 posSize);
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
			bool hitTest;

			Theme::GuiTheme* themeOverride;

			GuiInfo& guiInfo;

			bool hasFocus;
		};
	}
}

#endif