#ifndef TEXT_AREA_HPP
#define TEXT_AREA_HPP

/// Internal Includes
#include "ScrollBar.hpp"
#include "Text.hpp"
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {
			class TextArea : public GuiItem {
			public:

				TextArea(GuiInfo& info);
				virtual ~TextArea();

				void setMultiLine(bool multiLine);
				void setEditMode(bool edit);
				void setOnlyNumbers(bool onlyNum);

				void showLineNumbers(bool show);
				void setText(const Engine::Core::FormattedString& str);
				void addText(const Engine::Core::FormattedString& str);
				
				void setPassiveText(const Engine::Core::FormattedString& str);

				Engine::Core::FormattedString getText() const;

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				bool allowEdit;
				int editCursorLine;
				int editCursorChar;

				int cursorRenderXOffset;
				float cursorTimer;
				bool renderCursor;
				int cursorRenderYOffset;
				int cursorHeight;

				bool lineNumbers;
				bool multiLineEdit;
				bool isFocused;
				bool onlyNumbers;

				bool textUpdated;

				std::vector<Engine::Core::FormattedString> textData;

				ScrollBar* verticalScroll;
				
				int sizeYClipp;

				Texture::Texture2D* cursorTexture;
				Text text;

				Engine::Core::FormattedString passiveText;

			};
		}
	}
}

#endif
