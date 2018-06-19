#ifndef LIST_HPP
#define LIST_HPP

/// Internal Includes
#include "ScrollBar.hpp"
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"
#include "../../Core/String.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class ListItem : public GuiItem {
			public:
				Core::String identifier;
			};

			// returns true when a listItem has been matched
			typedef bool(*ListSearchFunction) (Core::String&, ListItem*);

			class List : public GuiItem {
			public:
				List();
				virtual ~List();

				void setTexture(Texture::Texture2D* texture);

				void addListItem(ListItem* listItem);

				void search(Core::String str, ListSearchFunction searchFunc);

				virtual void update(float dt, GuiHitInfo& hitInfo) override;
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

