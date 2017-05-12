#include "QueryDataExtension.hpp"

#include <Editor_Wrapper\IEditor.hpp>

#include "../CoreGlobals.hpp"
#include "../Assets/AssetManager.hpp"
#include "../Memory/MemoryManager.hpp"

class Simple : public IObject
{
public:
	// Inherited via IObject
	virtual const char * getName() override
	{
		return "Test";
	}
	virtual const int getFormID() override
	{
		return formID;
	}
	virtual void * getData() override
	{
		return nullptr;
	}

	const char* name;
	int formID;
	void* data;
};

void QueryDataExtension::execute(int nrArgs, ExtensionQueryDataEvent* args) {

	if (nrArgs != 1) {
		return;
	}

	if (args->objectType == OBJECT_TYPE_SCRIPT || args->objectType == OBJECT_TYPE_SCENE) {

		if (args->nrObjects == 0 && args->objectList == nullptr) {
			args->nrObjects = 2;
		}
		else {
			MasterFile* mst = &g_assetManager->masterFile;
			
			for (int i = 0; i < args->nrObjects; i++)
			{
				Simple* s = g_memoryManager->getTimeAllocator()->allocateTimedObject<Simple>(5.0f);
				s->formID = i;
				args->objectList[i] = s;
			}
		}
	}

}

IExtension<void>* QueryDataExtension::toExtensionPtr()
{
	return (IExtension<void>*)this;
}
