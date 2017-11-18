#ifndef TEXT_AREA_HPP
#define TEXT_AREA_HPP

/// Internal Includes
#include "Text.hpp"
#include "../GuiItem.hpp"
#include "ScrollBar.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {
			class TextArea : public GuiItem {
			public:

				TextArea();
				virtual ~TextArea();

				void setText(const Engine::Core::FormattedString& str);
				void addText(const Engine::Core::FormattedString& str);

				virtual void update(float dt) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				std::vector<Engine::Core::FormattedString> textData;

				ScrollBar* verticalScroll;
				Texture::Texture2D* scrollBg;
				Texture::Texture2D* scrollBar;
				
				int sizeYClipp;


				Text text;
			};
		}
	}
}

#endif
