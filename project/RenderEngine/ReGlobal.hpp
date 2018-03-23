#include <cstdint>

#include <functional>
#include <thread>

extern std::thread::id activeThread;
extern bool lockdown;

std::thread::id getThreadId();

#define GL_REQUEST_BUFFER       0x0001
#define GL_REQUEST_VERTEX_ARRAY 0x0002

void postGlCreateRequest(uint32_t type, uint32_t count, uint32_t* idPtr);
void postGlDeleteRequest(uint32_t type, uint32_t count, uint32_t* idPtr);

void processGlRequests();

void postGlFunction(std::function<void(void)>);
void processGlFunctions();