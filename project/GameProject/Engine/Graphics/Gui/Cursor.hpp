#ifndef CURSOR_HPP
#define CURSOR_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class Cursor : public GuiItem {

			public:

				Cursor();
				virtual ~Cursor();

				void setTexture(Texture::Texture2D* texture);

				virtual void update(float dt) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;


			private:
				Texture::Texture2D* tex;

			};
		}
	}
}

#endif