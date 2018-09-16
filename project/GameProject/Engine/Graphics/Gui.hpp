#ifndef GUI_HPP
#define GUI_HPP

/// Internal Includes
#include "GuiItem.hpp"
#include "ShaderContainer.hpp"
#include "Gui/Cursor.hpp"
#include "Gui/StatusBar.hpp"
#include "../Interfaces/Renderable.hpp"

/// External Includes
#include <EngineCore/AssetHandling/IAssetManager.hpp>
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {

		class CGui : public Renderable {
		public:

			CGui(Engine::AssetHandling::IAssetManager* assetManager);
			virtual ~CGui();

			void setVisible(bool _visible);

			void setCursor(Gui::Cursor* cursor);

			void setStatusBar(Gui::StatusBar* status);

			void setPosition(int x, int y);

			void addGuiItem(GuiItem* guiItem);
			void removeGuiItem(GuiItem* itemRef);
			
			bool mouseHitGui() const;
			void update(float dt);
			void render();

			Engine::AssetHandling::IAssetManager* getAssetManager() const;

		private:

			Engine::AssetHandling::IAssetManager* pAssetManager;

			Gui::Cursor* cur;
			Gui::StatusBar* statusBar;
			std::vector<GuiItem*> guiItems;

			GuiItem* focusedItem;

			GuiShaderContainer shaders;
			bool visible;
			glm::ivec2 pos;
		};
	}
}

#endif