#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <new>

#include <Editor_Wrapper\ExtensionMap.cpp>
#include <Editor_Wrapper\GetFormIDEvent.cpp>
#include <Editor_Wrapper\ObjectEvents.cpp>
#include <Editor_Wrapper\QueryEvent.cpp>
#include <Editor_Wrapper\SaveEvent.cpp>

#include <Editor_Wrapper\Window\MainWindowWrapper.cpp>

#include <Editor_Wrapper\EditorWindow.cpp>

#include <Editor_Wrapper\Editor.cpp>