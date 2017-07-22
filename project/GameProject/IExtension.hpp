#ifndef IEXTENSION_HPP
#define IEXTENSION_HPP

#include <cstdint>

// callback defines
#define SAVE_CALLBACK              0x00000001
#define GET_OBJECTS_CALLBACK       0x00000002
#define SET_SETTINGS_CALLBACK      0x00000003
#define ADD_OBJECT_CALLBACK		   0x00000004
#define DELETE_OBJECT_CALLBACK	   0x00000005
#define EDIT_OBJECT_CALLBACK	   0x00000006
#define GET_FORMID_CALLBACK		   0x00000007

// Object types
#define OBJECT_TYPE_STATIC         0x0001
#define OBJECT_TYPE_ANIM		   0x0002
#define OBJECT_TYPE_WEAPON		   0x0004
#define OBJECT_TYPE_AMMO		   0x0008
#define OBJECT_TYPE_DIALOG		   0x0010
#define OBJECT_TYPE_SCRIPT		   0x0020
#define OBJECT_TYPE_DECAL		   0x0040
#define OBJECT_TYPE_AUDIO		   0x0080
#define OBJECT_TYPE_SCENE		   0x0100
#define OBJECT_TYPE_RENDERLAYER	   0x0200

struct ExtensionSaveEvent {
	char* fileName;
	bool pack;
};

struct SceneStuff {
	float skyColor[4];
	bool hasFog;
	float fog[8];
	bool hasWater;
};

struct RenderLayerData {
	const char* name;
	char resolutionType;
	uint32_t width;
	uint32_t height;
	bool depthBuffer;
	bool stencilBuffer;
	char nrColorBuffers;
	uint32_t shaderProgramRef;
};

class IObject
{
public:
	virtual const char* getName() = 0;
	virtual const uint32_t getFormID() = 0;
	virtual void* getData() = 0;
	virtual uint32_t dataSize() = 0;
	virtual bool isDeleted() = 0;
};

struct ExtensionQueryDataEvent {
	uint32_t objectType;
	uint32_t nrObjects;
	IObject** objectList;
};

struct ExtensionAddItemEvent {
	uint32_t objectType;
	IObject* objectData;
};

struct ExtensionDeleteItemEvent {
	uint32_t formID;
};

struct ExtensionEditItemEvent {
	uint32_t objectType;
	IObject* objectData;
};

typedef ExtensionDeleteItemEvent ExtensionGetFormIDEvent;

template<typename T>
class IExtension {
public:
	virtual void execute(int nrArgs, T* arg) = 0;

	virtual IExtension<void>* toExtensionPtr() = 0;

};

#endif