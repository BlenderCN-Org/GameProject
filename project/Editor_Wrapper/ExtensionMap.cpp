#include "ExtensionMap.hpp"

std::map<int, IExtension<void>*> Extensions::extensionMap;
IAssetManager* Extensions::assetMan;
IEditorAccess* Extensions::editAccess;

namespace Extensions {
	void OnSceneChangeEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ formArg) {
		editAccess->setActiveScene(formArg->FormID);
	}
}