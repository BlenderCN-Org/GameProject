#include "../System/Sys.hpp"

#include "EditDataExtension.hpp"
#include "../CoreGlobals.hpp"
#include "../AssetManager.hpp"
#include "../Assets/DataObjects.hpp"

#include <Editor_Wrapper/IEditor.hpp>

void EditDataExtension::execute(int nrArgs, ExtensionEditItemEvent * arg) {

	if (nrArgs == 1 && arg != nullptr) {
		IObject* obj = arg->objectData;

		if (arg->objectType == 0) {

			Entry* e = gAssetManager->getEntry(obj->getFormID());

			if (strncmp(e->tag, RENDERLAYER_TAG, 4) == 0) {
				arg->objectType = OBJECT_TYPE_RENDERLAYER;
			} else if (strncmp(e->tag, SCENE_TAG, 4) == 0) {
				arg->objectType = OBJECT_TYPE_SCENE;
			}
		}

		if (arg->objectType == OBJECT_TYPE_SCENE) {

			Entry* e = gAssetManager->getEntry(obj->getFormID());
			if (e == nullptr) {
				return;
			}
			if (strncmp(e->tag, SCENE_TAG, 4) == 0) {
				memcpy(e->tag, SCENE_TAG, 4);

			}


		} else if (arg->objectType == OBJECT_TYPE_RENDERLAYER) {

			Entry* e = gAssetManager->getEntry(obj->getFormID());
			if (e == nullptr) {
				return;
			}
			if (strncmp(e->tag, RENDERLAYER_TAG, 4) == 0) {
				memcpy(e->tag, RENDERLAYER_TAG, 4);

				RenderLayerSaveData empty{};

				RenderLayerSaveData* rData = (RenderLayerSaveData*)obj->getData();
				if(rData == nullptr)
				{
					if (e->data) {
						rData = (RenderLayerSaveData*)e->data->getData();
						//rData = (RenderLayerData*)((IRenderLayerData*)e->data)->getRenderLayerData();
					}
					else {
						rData = &empty;
						rData->name = 0;
					}
				}
				const char* name = rData->name;
				const int32_t nameLength = getStringLength(name);
				void* data = malloc(sizeof(RenderLayerSaveData) + nameLength);
				RenderLayerSaveData* cData = (RenderLayerSaveData*)data;
				memcpy(cData, rData, sizeof(RenderLayerSaveData));
				if (nameLength) {
					cData->name = (char*)cData + sizeof(RenderLayerSaveData);
					memcpy((void*)cData->name, name, nameLength);
				}
				else {
					cData->name = nullptr;
				}
				//if (e->data) {
				//	delete e->data;
				//}

				//e->data = new DataObject(cData, sizeof(RenderLayerData) + nameLength);
				e->data = new RenderLayerDataObject(nullptr, 0);
				//RenderLayerSaveData sData;
				//sData.name = cData->name;
				//sData.width = cData->width;
				//sData.height = cData->height;
				//sData.depthBuffer = cData->depthBuffer;
				//sData.stencilBuffer = cData->stencilBuffer;
				//sData.resolutionType = cData->resolutionType;
				//sData.nrColorBuffers = cData->nrColorBuffers;
				//sData.shaderProgramRef = cData->shaderProgramRef;
				((RenderLayerDataObject*)e->data)->setRenderLayerData((RenderLayerSaveData*)cData);
				e->entrySize = sizeof(EntrySave) + e->data->getDataSize();

				gAssetManager->updateEntry(e);
				free(data);
			}
		}

	}

}

IExtension<void>* EditDataExtension::toExtensionPtr() {
	return (IExtension<void>*)this;
}
