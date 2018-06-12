#ifndef LIST_HPP
#define LIST_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"
#include "ScrollBar.hpp"
#include "../../Core/String.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class ListItem : public GuiItem {

			};

			class List : public GuiItem {
			public:
				List();
				virtual ~List();

				void setTexture(Texture::Texture2D* texture);

				void addListItem(ListItem* listItem);

				void search(Core::String str);

				virtual void update(float dt) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:
				
				Core::String searchString;

				std::vector<ListItem*> listItems;
				Texture::Texture2D* tex;
				ScrollBar* verticalScroll;
			};
		}
	}
}


#endif

