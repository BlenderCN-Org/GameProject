#ifndef WINDOWSYSTEM_HPP
#define WINDOWSYSTEM_HPP

#ifdef WINDOWSYSTEM_EXPORTS
#define WINDOWSYSTEM_API __declspec(dllexport)
#else
#define WINDOWSYSTEM_API __declspec(dllimport)
#endif

typedef struct WSWindow WSWindow;
typedef struct WSMonitor WSMonitor;

WINDOWSYSTEM_API bool wsInit();
WINDOWSYSTEM_API bool wsTerminate();

WINDOWSYSTEM_API WSWindow* wsCreateWindow(int width, int height, const char* title, WSMonitor* monitor, WSWindow* share);
WINDOWSYSTEM_API void wsPollEvents(WSWindow* window);
WINDOWSYSTEM_API void wsDestroyWindow(WSWindow* window);
WINDOWSYSTEM_API bool wsIsWindowVisible(WSWindow* window);

WINDOWSYSTEM_API void wsCreateOGLContext(WSWindow* window);
WINDOWSYSTEM_API void wsMakeCurrent(WSWindow* window);
WINDOWSYSTEM_API void wsSwapBuffers(WSWindow* window);

#endif