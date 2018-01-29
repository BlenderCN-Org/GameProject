#ifndef LABEL_HPP
#define LABEL_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"
#include "Text.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {
			class Label : public GuiItem {
			public:
				Label();
				virtual ~Label();

				int calcTextWidth();

				void setText(const Engine::Core::FormattedString& str);

				virtual void update(float dt) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:
				Text text;
			};
		}
	}
}

#endif
