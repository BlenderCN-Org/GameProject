
/// Internal Includes
#include "ObjectList.hpp"

/// External Includes

/// Std Includes

const int SEARCH_FIELD_HEIGHT = 17;

bool search(Engine::Core::String& searchStr, Engine::Graphics::Gui::ListItem* listItem) {
	if (strcmp(listItem->identifier.cStr() ,searchStr.cStr()) == 0) {
		return true;
	}
	return false;
}

ObjectList::ObjectList() {
	objectList = new Engine::Graphics::Gui::List();
	searchField = new Engine::Graphics::Gui::TextArea();

	setupComponents();

}

ObjectList::~ObjectList() {
	delete objectList;
	delete searchField;
}

void ObjectList::update(float dt, Engine::Graphics::GuiHitInfo& hitInfo) {

	if (visible) {

		searchField->setSize(size.x - 2, SEARCH_FIELD_HEIGHT - 2);
		objectList->setSize(size.x, size.y - SEARCH_FIELD_HEIGHT);
		Panel::update(dt, hitInfo);

		objectList->search(searchField->getText().toString(), search);

	}
}


void ObjectList::render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) {

	if (visible) {
		Panel::render(vpMatRef, shaderContainer);
	}
}


void ObjectList::setupComponents() {

	searchField->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	objectList->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);

	searchField->setSize(0, SEARCH_FIELD_HEIGHT);
	searchField->setPosition(1, 1);
	searchField->setMultiLine(false);
	searchField->setEditMode(true);
	searchField->showLineNumbers(false);

	searchField->setText("hello");

	objectList->setSize(0, -SEARCH_FIELD_HEIGHT);
	objectList->setPosition(0, SEARCH_FIELD_HEIGHT);

	searchField->setVisible(true);
	objectList->setVisible(true);

	addGuiItem(searchField);
	//addGuiItem(objectList);

}