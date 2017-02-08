#ifndef IEDITOR_HPP
#define IEDITOR_HPP

#ifdef EDITOR_WRAPPER_EXPORTS
#define EDITOR_API _declspec(dllexport)
#else
#define EDITOR_API _declspec(dllimport)
#endif

class IEditor {

public:

	virtual void initializeEditor() = 0;
	virtual void releaseEditor() = 0;

private:

};

extern "C" EDITOR_API IEditor* CreateEditor();

typedef IEditor* (*CreateEditorProc)();

#endif
