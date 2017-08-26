#ifndef IEDITOR_HPP
#define IEDITOR_HPP

#ifdef EDITOR_WRAPPER_EXPORTS
#define EDITOR_API _declspec(dllexport)
#else
#define EDITOR_API _declspec(dllimport)
#endif

#include "../GameProject/IAssetManager.hpp"
#include "../GameProject/IEditorAccess.hpp"
#include "../GameProject/IExtension.hpp"
#include <RenderEngine/IRenderEngine.hpp>
#include <RenderEngine/IWindow.hpp>

enum EditorStatus {
	UNINITIALIZED,	// editor not initialized, call initializeEditor
	STOPPED,		// no operations allowed, call start on editor
	STARTING,		// editor is starting up, show loading message or something
	RUNNING,		// editor is running, you can edit stuff
	HIDDEN,			// editor is running in background
	STOPPING,		// no operations allowed, editor is closing
};

class IEditor {
public:

	virtual bool initializeEditor(IRenderEngine* re) = 0;
	virtual void releaseEditor() = 0;

	virtual void startEditor() = 0;
	virtual void stopEditor() = 0;

	virtual void registerExtension(int callbackIndex, IExtension<void>* ext) = 0;

	virtual EditorStatus getStatus() const = 0;

	virtual void update() = 0;

	virtual IWindow* getEditorWindow() = 0;

	virtual void postPixels(uint32_t width, uint32_t height) = 0;

	virtual void setAssetManager(IAssetManager* assetMan) = 0;
	virtual void setEditorAccess(IEditorAccess* editAccess) = 0;

private:
};

extern "C" EDITOR_API IEditor* CreateEditor();

typedef IEditor* (*CreateEditorProc)();

#endif