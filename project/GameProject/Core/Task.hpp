#ifndef TASK_HPP
#define TASK_HPP

#include <cstdint>

enum class TaskType
{
	eTaskType_threadTerminate = 0,
	eTaskType_loadAsset = 1,
	eTaskType_execute = 2,
};

enum class LoadType
{
	eLoadType_file = 0,
	eLoadType_memory = 1,
	eLoadType_network = 2,
};

struct LoadInfo
{
	LoadType type;

	char* fileName; // loadFromFile

	void* memoryPtr; // loadFrom memory
	uint32_t memoryDataSize; // loadFrom memory

	uint32_t assetServerID; // loadFrom network
};

struct AssetLoadStruct
{
	LoadInfo howToLoad;
};

struct Task
{
	TaskType type;

	void* data; // pointer object that is to be processed
};

#endif