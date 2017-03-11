#ifndef IEDITOR_HPP
#define IEDITOR_HPP

#ifdef EDITOR_WRAPPER_EXPORTS
#define EDITOR_API _declspec(dllexport)
#else
#define EDITOR_API _declspec(dllimport)
#endif

class IEditor {

public:

	virtual bool initializeEditor() = 0;
	virtual void releaseEditor() = 0;

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
