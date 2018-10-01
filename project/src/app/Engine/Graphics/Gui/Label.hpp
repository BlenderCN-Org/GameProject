#ifndef LABEL_HPP
#define LABEL_HPP

/// Internal Includes
#include "Text.hpp"
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {
			class Label : public GuiItem {
			public:
				Label(GuiInfo info);
				virtual ~Label();

				int calcTextWidth() const;
				int calcTextHeight() const;

				void setIcon(Engine::Graphics::Texture::Texture2D* ico);

				void setText(const Engine::Core::FormattedString& str);

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				Engine::Graphics::Texture::Texture2D* icon;

				Text text;
			};
		}
	}
}

#endif
