#ifndef OBJECT_LIST_HPP
#define OBJECT_LIST_HPP

/// Internal Includes
#include "../../../Engine/Graphics/GuiItem.hpp"
#include "../../../Engine/Graphics/ShaderContainer.hpp"

#include "../../../Engine/Graphics/Gui/List.hpp"
#include "../../../Engine/Graphics/Gui/Panel.hpp"
#include "../../../Engine/Graphics/Gui/TextArea.hpp"

/// External Includes

/// Std Includes

class ObjectList : public Engine::Graphics::Gui::Panel {

public:
	
	ObjectList();
	virtual ~ObjectList();

	virtual void update(float dt, Engine::Graphics::GuiHitInfo& hitInfo);
	virtual void render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer);


private:

	void setupComponents();

	Engine::Graphics::Gui::List* objectList;
	Engine::Graphics::Gui::TextArea* searchField;
};

#endif