#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include "../GuiItem.hpp"

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class GuiWindow : public GuiItem {

			public:
				GuiWindow();
				virtual ~GuiWindow();

				virtual void update(float dt);
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer);

			private:



			};

		}
	}
}

#endif