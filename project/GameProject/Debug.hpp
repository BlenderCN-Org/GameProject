#ifndef GAME_DEBUG_HPP
#define GAME_DEBUG_HPP

#ifdef _DEBUG

void loadRenderDocGpuDebugger();
void unloadRenderDocGpuDebugger();
#else
#define loadRenderDocGpuDebugger()
#define unloadRenderDocGpuDebugger()
#endif


#endif