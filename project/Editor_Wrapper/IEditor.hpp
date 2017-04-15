#ifndef IEDITOR_HPP
#define IEDITOR_HPP

#ifdef EDITOR_WRAPPER_EXPORTS
#define EDITOR_API _declspec(dllexport)
#else
#define EDITOR_API _declspec(dllimport)
#endif

#include "../GameProject/IExtension.hpp"

// callback defines
#define SAVE_CALLBACK              0x00000001
#define GET_OBJECTS_CALLBACK       0x00000002
#define SET_SETTINGS_CALLBACK      0x00000003
#define ADD_OBJECT_CALLBACK		   0x00000004

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

class IEditor {

public:

	virtual bool initializeEditor() = 0;
	virtual void releaseEditor() = 0;

	virtual void registerExtension(int callbackIndex, IExtension<void>* ext) = 0;

	virtual void detach() = 0;
	virtual void attach() = 0;

	virtual bool isRunning() = 0;

	virtual void poll() = 0;

	virtual void setGameWindow(void* windowPtr) = 0;

private:

};

extern "C" EDITOR_API IEditor* CreateEditor();

typedef IEditor* (*CreateEditorProc)();

#endif
