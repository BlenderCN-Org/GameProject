#ifndef GUI_ITEM_HPP
#define GUI_ITEM_HPP

/// Internal Includes
#include "ShaderContainer.hpp"
#include "GuiTheme.hpp"

/// External Includes
#include <AssetHandling/IAssetManager.hpp>
#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

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

		struct ScreenRegion {
			glm::ivec2 pos;
			glm::ivec2 size;
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

			void getSize(int &w, int &h);

			bool isVisible() const;

			void setVisible(bool visibilityFlag);
			void setZIndex(int zIndex);

			void setHitTest(bool enable);

			void updateRenderRegion(ScreenRegion region);

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

			glm::ivec2 relativePosition; // position relative to parent
			glm::ivec2 size;             // the actual size of the component
			glm::ivec2 absolutePosition; // screen position

			ScreenRegion screenRegion;   // region where item is allowed to draw into

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