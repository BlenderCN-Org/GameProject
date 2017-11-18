#ifndef SCROLL_BAR_HPP
#define SCROLL_BAR_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

namespace Engine {
	namespace Graphics {
		namespace Gui {

			enum class ScrollBarDirection {
				SCROLL_HORIZONTAL,
				SCROLL_VERTICAL
			};

			class ScrollBar : public GuiItem {
			public:

				ScrollBar();
				virtual ~ScrollBar();

				void setAutoScroll(bool autoScroll);

				void setScrollDirection(ScrollBarDirection dir);
				void setNumElements(size_t num);
				void setMinElements(size_t num);

				size_t getSelectedElement() const;

				void setBackgroundTexture(Texture::Texture2D* texture);
				void setScrollbarTexture(Texture::Texture2D* texture);

				virtual void update(float dt) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				void updateBarPosition(int mx, int my);

				ScrollBarDirection scrollDir;

				size_t numElements;
				size_t minElements;

				size_t selectedElement;
				bool autoScrollLastElement;

				int scrollbarScreenPosition;
				int scrollbarLastPos;
				int scrollbarThickness;

				Texture::Texture2D* backgroundTexture;
				Texture::Texture2D* scrollbarTexture;;

				bool holdingBar;
				bool forceUpdate;
			};

		}
	}
}

#endif
