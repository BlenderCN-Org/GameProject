#ifndef GUI_HPP
#define GUI_HPP

/// Internal Includes
#include "GuiItem.hpp"
#include "ShaderContainer.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {

		class CGui {
		public:

			CGui();
			virtual ~CGui();

			void addGuiItem(GuiItem* guiItem);

			void render();

		private:
			std::vector<GuiItem*> guiItems;

			GuiShaderContainer shaders;

		};
	}
}

#endif