#ifndef GUI_THEME_HPP
#define GUI_THEME_HPP

/// Internal Includes
#include "Textures/Texture2D.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes

namespace Engine {
	namespace Theme {

		struct ButtonTheme {
			Engine::Graphics::Texture::Texture2D* textureNormal;
			Engine::Graphics::Texture::Texture2D* textureHovering;
			Engine::Graphics::Texture::Texture2D* texturePressing;
		};

		struct ComboBoxTheme {
			Engine::Graphics::Texture::Texture2D* textureNormal;
		};

		struct PanelTheme {
			Engine::Graphics::Texture::Texture2D* textureNormal;
		};

		struct ScrollBarTheme {
			Engine::Graphics::Texture::Texture2D* textureBackground;
			Engine::Graphics::Texture::Texture2D* textureBar;
		};

		struct ProgressBarTheme {
			Engine::Graphics::Texture::Texture2D* textureBackground;
			Engine::Graphics::Texture::Texture2D* textureBar;
		};
		
		struct TextAreaTheme {
			Engine::Graphics::Texture::Texture2D* textureBackground;
			ScrollBarTheme areaScroll;
		};

		struct ListTheme {
			Engine::Graphics::Texture::Texture2D* textureBackground;
			Engine::Graphics::Texture::Texture2D* textureItem;
			Engine::Graphics::Texture::Texture2D* textureSelectedItem;
			ScrollBarTheme areaScroll;
		};

		struct StatusBarTheme {
			Engine::Graphics::Texture::Texture2D* textureBackground;
		};

		struct TabViewTheme {
			Engine::Graphics::Texture::Texture2D* background;
			Engine::Graphics::Texture::Texture2D* icoNext;
			Engine::Graphics::Texture::Texture2D* icoPrev;
		};

		struct GuiTheme {

			glm::vec4 textColor;
			ButtonTheme button;
			ComboBoxTheme comboBox;
			PanelTheme panel;
			ScrollBarTheme scrollBar;
			ProgressBarTheme progressBar;
			TextAreaTheme textArea;
			ListTheme list;
			StatusBarTheme statusBar;
			TabViewTheme tabView;
		};
	}
}

#endif