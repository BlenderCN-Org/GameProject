/// Internal Includes
#include "../../Engine/Graphics/GuiItem.hpp"
#include "../../Engine/Graphics/ShaderContainer.hpp"

#include "../../Engine/Graphics/Gui/List.hpp"
#include "../../Engine/Graphics/Gui/Panel.hpp"
#include "../../Engine/Graphics/Gui/TextArea.hpp"
#include "../../Engine/Graphics/Gui/Label.hpp"
#include "../../Engine/Graphics/Gui/Button.hpp"

/// External Includes
#include <Core/System.hpp>

/// Std Includes
#include <vector>

enum class SelectStatus {

	SELECT_STATUS_WAITING,
	SELECT_STATUS_CONFIRM,
	SELECT_STATUS_CANCEL
};

class FileDialog : public Engine::Graphics::Gui::Panel {

public:
	FileDialog(Engine::Graphics::GuiInfo& info);
	virtual ~FileDialog();

	virtual void update(float dt, Engine::Graphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
	virtual void render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) override;

	void showDialog(Engine::Core::String initialDirectory, bool forcedDir = false);
	
	SelectStatus getSelectStatus() const;

	Engine::Core::String getFileName() const;

private:

	void setupComponents();
	void setupFileList(std::vector<Engine::System::FileDesc> files, bool removeDirectories);

	std::vector<Engine::Graphics::Gui::ListItem*> myList;

	SelectStatus status;

	Engine::Graphics::Gui::List* fileList;
	Engine::Graphics::Gui::TextArea* currentDir;
	Engine::Graphics::Gui::TextArea* searchBox;
	Engine::Graphics::Gui::Button* cancelButton;
	Engine::Graphics::Gui::Button* selectButton;

	Engine::Graphics::Texture::Texture2D* fileTexture;
	Engine::Graphics::Texture::Texture2D* dirTexture;

};