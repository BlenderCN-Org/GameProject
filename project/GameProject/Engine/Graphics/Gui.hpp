#ifndef GUI_HPP
#define GUI_HPP

/// Internal Includes
#include "../Interfaces/Renderable.hpp"
#include "GuiItem.hpp"
#include "ShaderContainer.hpp"
#include "Gui/Cursor.hpp"

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
			
			bool mouseHitGui() const;
			void update(float dt);
			void render();

		private:
			Gui::Cursor* cur;
			std::vector<GuiItem*> guiItems;

			GuiShaderContainer shaders;
			bool visible;
			glm::ivec2 pos;
		};
	}
}

#endif