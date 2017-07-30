#include "EditDataExtension.hpp"
#include "../CoreGlobals.hpp"
#include "../AssetManager.hpp"

#include <Editor_Wrapper\IEditor.hpp>

void EditDataExtension::execute(int nrArgs, ExtensionEditItemEvent * arg) {
	
	if (nrArgs == 1 && arg != nullptr) {
		IObject* obj = arg->objectData;

		if (arg->objectType == OBJECT_TYPE_SCENE) {

		} else if (arg->objectType == OBJECT_TYPE_RENDERLAYER) {

			Entry* e = gAssetManager->getEntry(obj->getFormID());
			memcpy(e->tag, RENDERLAYER_TAG, 4);

			RenderLayerData* rData = (RenderLayerData*)obj->getData();
			RenderLayerData* cData = new RenderLayerData();
			memcpy(cData, rData, sizeof(RenderLayerData));
			if (e->data) {
				delete e->data;
			}
			e->data = cData;
			e->entrySize = sizeof(EntrySave) + sizeof(RenderLayerData);

			gAssetManager->updateEntry(e);
		}

	}

}

IExtension<void>* EditDataExtension::toExtensionPtr() {
	return (IExtension<void>*)this;
}
