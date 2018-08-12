#ifndef LIST_HPP
#define LIST_HPP

/// Internal Includes
#include "ScrollBar.hpp"
#include "Text.hpp"
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes
#include <EngineCore/Core/String.hpp>

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class ListItem : public GuiItem{
			public:

				ListItem(GuiInfo& info);
				~ListItem();

				void setText(Core::String t);
				glm::ivec2 getAbsPos() const;

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;


				Core::String identifier;
				Text text;
				Engine::Graphics::Texture::Texture2D* icon;

				bool isFocused;

			};

			// returns true when a listItem has been matched
			typedef bool(*ListSearchFunction) (Core::String&, ListItem*);

			class List : public GuiItem {
			public:
				List(GuiInfo& info);
				virtual ~List();

				void setTexture(Texture::Texture2D* texture);

				void addListItem(ListItem* listItem);
				void removeListItem(ListItem* listItem);
				void clearList();

				ListItem* getSelectedItem() const;

				void search(Core::String str, ListSearchFunction searchFunc);

				virtual void update(float dt, GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:
				
				Core::String searchString;

				bool hasSearch;

				ListItem* selectedItem;

				std::vector<ListItem*> listItems;
				std::vector<ListItem*> seaList;
				std::vector<ListItem*> dispList;
				Texture::Texture2D* tex;
				ScrollBar* verticalScroll;

			};
		}
	}
}


#endif

