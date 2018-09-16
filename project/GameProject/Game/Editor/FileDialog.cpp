/// Internal Includes
#include "FileDialog.hpp"

#include "Resources/file_x16.png.h"
#include "Resources/folder_x16.png.h"

/// External Includes
#include <EngineCore/Core/System.hpp>
#include <AssetLib/AssetLib.hpp>

/// Std Includes

const int SEARCH_FIELD_HEIGHT = 20;
const int SEARCH_BOX_SIZE = 50;

bool fileSearch(Engine::Core::String& searchStr, Engine::Graphics::Gui::ListItem* listItem) {
	if (listItem->identifier.contains(searchStr)) {
		return true;
	}
	return false;
}

FileDialog::FileDialog(Engine::Graphics::GuiInfo& info) : Panel(info) {
	fileList = new Engine::Graphics::Gui::List(info);
	searchBox = new Engine::Graphics::Gui::TextArea(info);
	currentDir = new Engine::Graphics::Gui::TextArea(info);
	cancelButton = new Engine::Graphics::Gui::Button(info);
	selectButton = new Engine::Graphics::Gui::Button(info);

	fileTexture = new Engine::Graphics::Texture::Texture2D();
	dirTexture = new Engine::Graphics::Texture::Texture2D();

}

FileDialog::~FileDialog() {

	for (size_t i = 0; i < myList.size(); i++) {
		delete myList[i];
	}
	myList.clear();

	delete fileTexture;
	delete dirTexture;

	delete fileList;
	delete searchBox;
	delete currentDir;
	delete cancelButton;
	delete selectButton;
}

void FileDialog::update(float dt, Engine::Graphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) {

	if (visible) {

		searchBox->setSize(size.x - 75, SEARCH_FIELD_HEIGHT - 2);
		currentDir->setSize(size.x - 75, SEARCH_FIELD_HEIGHT - 2);
		fileList->setSize(size.x, size.y - ((2 * SEARCH_FIELD_HEIGHT)));
		Panel::update(dt, hitInfo, currentFocus);

		fileList->search(searchBox->getText().toString(), fileSearch);

		if (selectButton->wasPressed()) {
			printf("Button pressed\n");

			Engine::Graphics::Gui::ListItem* selected = fileList->getSelectedItem();
			if (nullptr != selected) {
				//printf("Selected item %s\n", selected->identifier.cStr());
				status = SelectStatus::SELECT_STATUS_CONFIRM;
			} else {
				//printf("Select an item you dingus");
			}
		}
		if (cancelButton->wasPressed()) {
			status = SelectStatus::SELECT_STATUS_CANCEL;
		}

	}
}

void FileDialog::render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) {
	Panel::render(vpMatRef, shaderContainer);
}

void FileDialog::showDialog(Engine::Core::String initialDirectory, bool forcedDir) {

	for (size_t i = 0; i < myList.size(); i++) {
		delete myList[i];
	}
	myList.clear();
	fileList->clearList();

	status = SelectStatus::SELECT_STATUS_WAITING;

	std::string str = Engine::System::removeDuplicateSlashes(initialDirectory.cStr());

	std::vector<Engine::System::FileDesc> files = Engine::System::listFiles(str.c_str());

	setupComponents();

	setupFileList(files, forcedDir);

	currentDir->setText(str.c_str());

	setVisible(true);
}

SelectStatus FileDialog::getSelectStatus() const {
	return status;
}

Engine::Core::String FileDialog::getFileName() const {
	Engine::Graphics::Gui::ListItem* selected = fileList->getSelectedItem();
	Engine::Core::String str = currentDir->getText().toString();
	str += selected->identifier;
	return str;
}

void FileDialog::setupComponents() {

	searchBox->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	fileList->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);

	searchBox->setSize(50, SEARCH_FIELD_HEIGHT);
	searchBox->setPosition(1, SEARCH_FIELD_HEIGHT);
	searchBox->setMultiLine(false);
	searchBox->setEditMode(true);
	searchBox->showLineNumbers(false);
	Engine::Core::FormattedString passiveTex = "Search";
	passiveTex.formatString(0, -1, glm::vec4(0.5F));
	searchBox->setPassiveText(passiveTex);

	searchBox->setText("");

	fileList->setSize(50, 50);
	fileList->setPosition(0, 2 * SEARCH_FIELD_HEIGHT);

	currentDir->setPosition(1, 1);
	currentDir->setSize(50, SEARCH_FIELD_HEIGHT);
	currentDir->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	currentDir->setMultiLine(false);
	currentDir->setEditMode(true);
	currentDir->showLineNumbers(false);

	searchBox->setVisible(true);
	fileList->setVisible(true);
	currentDir->setVisible(true);

	selectButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_RIGHT);
	selectButton->setSize(75, 20);
	selectButton->setPosition(0, 0);
	selectButton->setVisible(true);
	selectButton->setText("Open");

	cancelButton->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_RIGHT);
	cancelButton->setSize(75, 20);
	cancelButton->setPosition(0, 20);
	cancelButton->setVisible(true);
	cancelButton->setText("Cancel");

	addGuiItem(cancelButton);
	addGuiItem(selectButton);
	addGuiItem(currentDir);
	addGuiItem(searchBox);
	addGuiItem(fileList);

	//std::string filePath = "E:\\GameProjectAssets\\Data\\Textures\\Gui\\file_x16.png";
	int w, h, c;
	//void* data = AssetLib::loadTexture(filePath.c_str(), w, h, c);
	void* data = AssetLib::loadTextureFromMemory((void*)fileIcon_x16, sizeof(fileIcon_x16), w, h, c);
	fileTexture->setData(w, h, c, data);
	AssetLib::freeImageData(data);

	//filePath = "E:\\GameProjectAssets\\Data\\Textures\\Gui\\folder_x16.png";
	//data = AssetLib::loadTexture(filePath.c_str(), w, h, c);
	data = AssetLib::loadTextureFromMemory((void*)folderIcon_x16, sizeof(folderIcon_x16), w, h, c);
	dirTexture->setData(w, h, c, data);
	AssetLib::freeImageData(data);

}

void FileDialog::setupFileList(std::vector<Engine::System::FileDesc> files, bool removeDirectories) {

	for (size_t i = 0; i < files.size(); i++) {

		if (true == removeDirectories) {
			if (true == files[i].isDirectory) {
				continue;
			}
		} else if ("." == files[i].name) {
			continue;
		}

		auto listItem = new Engine::Graphics::Gui::ListItem(guiInfo);
		listItem->setSize(200, 200);
		Engine::Core::String str = files[i].name.c_str();
		//str += " (";
		//str += std::to_string(files[i].fileSize).c_str();
		//str += ")";

		listItem->setText(str);
		if (files[i].isDirectory) {
			listItem->icon = dirTexture;
		} else {
			listItem->icon = fileTexture;
		}

		fileList->addListItem(listItem);
		myList.push_back(listItem);
	}

}