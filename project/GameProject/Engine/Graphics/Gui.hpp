#ifndef GUI_HPP
#define GUI_HPP

/// Internal Includes
#include "GuiItem.hpp"
#include "ShaderContainer.hpp"
#include "Gui/Cursor.hpp"
#include "../Interfaces/Renderable.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {

		class CGui : public Renderable {
		public:

			CGui();
			virtual ~CGui();

			void setVisible(bool _visible);

			void setCursor(Gui::Cursor* cursor);

			void setPosition(int x, int y);

			void addGuiItem(GuiItem* guiItem);
			void removeGuiItem(GuiItem* itemRef);
			
			bool mouseHitGui() const;
			void update(float dt);
			void render();

		private:
			Gui::Cursor* cur;
			std::vector<GuiItem*> guiItems;

			GuiItem* focusedItem;

			GuiShaderContainer shaders;
			bool visible;
			glm::ivec2 pos;
		};
	}
}

#endif