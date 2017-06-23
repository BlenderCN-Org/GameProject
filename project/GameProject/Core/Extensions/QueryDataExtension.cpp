#include "QueryDataExtension.hpp"

#include <Editor_Wrapper\IEditor.hpp>

#include "../CoreGlobals.hpp"
#include "../Assets/AssetManager.hpp"
#include "../Memory/MemoryManager.hpp"

#include "../Assets/Forms/SceneForm.hpp"


class Simple : public IObject {
public:
	// Inherited via IObject
	virtual const char * getName() override {
		return name;
	}
	virtual const uint32_t getFormID() override {
		return formID;
	}
	virtual void * getData() override {
		return data;
	}
	virtual uint32_t dataSize() override {
		return sizeize;
	}
	virtual bool isDeleted() override {
		return deleted;
	}

	const char* name = nullptr;
	uint32_t formID = 0;
	void* data = nullptr;
	uint32_t sizeize = 0;
	bool deleted = false;
};

struct Name {
	char n[128];
};

void QueryDataExtension::execute(int nrArgs, ExtensionQueryDataEvent* args) {

	if (nrArgs != 1) {
		return;
	}

	MasterFile* mst = &g_assetManager->masterFile;

	if (args->objectType == OBJECT_TYPE_SCRIPT) {

		if (args->nrObjects == 0 && args->objectList == nullptr) {
			args->nrObjects = 2;
		} else {

			for (uint32_t i = 0; i < args->nrObjects; i++) {
				Simple* s = g_memoryManager->getTimeAllocator()->allocateTimedObject<Simple>(5.0f);
				s->formID = i;
				args->objectList[i] = s;
			}
		}
	} else if (args->objectType == OBJECT_TYPE_SCENE) {

		if (args->nrObjects == 0 && args->objectList == nullptr) {
			args->nrObjects = mst->getNumberUsingType(args->objectType);
		} else {

			for (uint32_t i = 0; i < args->nrObjects; i++) {
				Simple* s = g_memoryManager->getTimeAllocator()->allocateTimedObject<Simple>(5.0f);
				s->formID = mst->getFormIDFromTypeAndIndex(args->objectType, i);
				SceneForm* frm = (SceneForm*)mst->getForm(s->formID);
				s->name = (char*)g_memoryManager->getTimeAllocator()->allocateTimedObject<Name>(5.0f);
				s->deleted = mst->isFormDeleted(s->formID);

				std::string str = std::string(frm->getName());

				SceneStuff* sc = g_memoryManager->getTimeAllocator()->allocateTimedObject<SceneStuff>(5.0f);
				sc->hasFog = frm->fogEnabled;
				sc->hasWater = frm->enableWater;
				sc->skyColor[0] = frm->skyColor.r;
				sc->skyColor[1] = frm->skyColor.g;
				sc->skyColor[2] = frm->skyColor.b;
				sc->skyColor[3] = frm->skyColor.a;

				sc->fog[0] = frm->fogColorNear.r;
				sc->fog[1] = frm->fogColorNear.g;
				sc->fog[2] = frm->fogColorNear.b;
				sc->fog[3] = frm->fogColorNear.a;

				sc->fog[4] = frm->fogColorFar.r;
				sc->fog[5] = frm->fogColorFar.g;
				sc->fog[6] = frm->fogColorFar.b;
				sc->fog[7] = frm->fogColorFar.a;

				memset((void*)s->name, 0, sizeof(Name));
				memcpy((void*)s->name, str.c_str(), str.size() > sizeof(Name) ? sizeof(Name) : str.size());

				s->data = sc;
				args->objectList[i] = s;
			}
		}
	}

}

IExtension<void>* QueryDataExtension::toExtensionPtr() {
	return (IExtension<void>*)this;
}
