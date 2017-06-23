#ifndef MEMORYMAN_HPP
#define MEMORYMAN_HPP

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#ifndef DBG_NEW

#include <new>


void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* ptr);
void operator delete[](void* ptr);

#endif // DBG_NEW

void InitMemoryManagement();
void ReleaseMemoryManagement();

#endif