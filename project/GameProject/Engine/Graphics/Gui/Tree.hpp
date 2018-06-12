#ifndef TREE_HPP
#define TREE_HPP

/// Internal Includes
#include "../GuiItem.hpp"
#include "../Textures/Texture2D.hpp"

/// External Includes

/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Gui {

			class TreeItem : public GuiItem {

			};

			class Tree : public GuiItem {
			public:
				Tree();
				virtual ~Tree();

				void setTexture(Texture::Texture2D* texture);

				void addGuiItem(TreeItem* guiItem);

				virtual void update(float dt) override;
				virtual void render(glm::mat4 &vpMatRef, GuiShaderContainer& shaderContainer) override;

			private:

				std::vector<TreeItem*> treeItems;
				Texture::Texture2D* tex;
			};
		}
	}
}

#endif