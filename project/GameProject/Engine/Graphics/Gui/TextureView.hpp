#ifndef TEXtURE_VIEW_HPP
#define TEXtURE_VIEW_HPP

#include "../GuiItem.hpp"
#include "Panel.hpp"
#include "ScrollBar.hpp"

#include "../Textures/Texture2DReference.hpp"


namespace Engine {
	namespace Graphics {
		namespace Gui {

			class TextureView : public GuiItem {

			public:

				TextureView();
				virtual ~TextureView();

				void setTexture(Texture::Texture2DReference* textureRef);

				virtual void update(float dt);
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer);

			private:

				Texture::Texture2D* scrollBg;
				Texture::Texture2D* scrollBar;

				Texture::Texture2D* mainText;

				ScrollBar* vertBar;
				ScrollBar* horzBar;

				Texture::Texture2DReference* tex;

				uint32_t texWidth;
				uint32_t texHeight;

				uint32_t offsetX;
				uint32_t offsetY;

				int oldX;
				int oldY;

			};

		}
	}
}

#endif