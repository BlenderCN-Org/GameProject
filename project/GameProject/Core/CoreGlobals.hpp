#ifndef COREGLOBALS_HPP
#define COREGLOBALS_HPP

class Core;
class IRenderEngine;
class AssetManager;
class MemoryManager;
class ThreadManager;

extern Core* g_core;
extern IRenderEngine* g_renderEngine;
extern AssetManager* g_assetManager;
extern MemoryManager* g_memoryManager;
extern ThreadManager* g_threadManager;

#endif