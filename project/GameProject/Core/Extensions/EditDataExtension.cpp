#include "EditDataExtension.hpp"
#include "../Assets/MasterFile.hpp"
#include "../CoreGlobals.hpp"
#include "../Assets/AssetManager.hpp"
#include <Editor_Wrapper\IEditor.hpp>
#include "../Assets/Forms/SceneForm.hpp"

void EditDataExtension::execute(int nrArgs, ExtensionEditItemEvent * arg) {

	if (nrArgs == 1 && arg != nullptr) {
		IObject* obj = arg->objectData;

		MasterFile* mst = &g_assetManager->getAssetManager()->masterFile;

		BaseForm* frm = mst->getForm(obj->getFormID());
		if (frm->getType() == OBJECT_TYPE_SCENE) {
			SceneForm* scene = (SceneForm*)frm;
			SceneStuff* sc = (SceneStuff*)obj->getData();
			
			std::string name = std::string(obj->getName());

			memcpy(scene->name, name.c_str(), name.size() > sizeof(scene->name) ? 128 : name.size() );

			scene->skyColor.r = sc->skyColor[0];
			scene->skyColor.g = sc->skyColor[1];
			scene->skyColor.b = sc->skyColor[2];
			scene->skyColor.a = sc->skyColor[3];

			scene->fogEnabled = sc->hasFog;

			scene->fogColorNear.r = sc->fog[0];
			scene->fogColorNear.g = sc->fog[1];
			scene->fogColorNear.b = sc->fog[2];
			scene->fogColorNear.a = sc->fog[3];

			scene->fogColorFar.r = sc->fog[4];
			scene->fogColorFar.g = sc->fog[5];
			scene->fogColorFar.b = sc->fog[6];
			scene->fogColorFar.a = sc->fog[7];

			scene->enableWater = sc->hasWater;
			
		}

	}

}

IExtension<void>* EditDataExtension::toExtensionPtr() {
	return (IExtension<void>*)this;
}
