#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

/// Internal Includes

/// External Includes

/// Std Includes
#include <memory>

#define KB 1024
#define MB KB*KB

namespace Engine {
	namespace Core {

		class FrameAllocator {

			FrameAllocator(size_t heapSize) : allocatedSize(0U), usedSize(0U), heap(nullptr) {

				heap = malloc(heapSize);
				allocatedSize = heapSize;
			}

			virtual ~FrameAllocator() {
				usedSize = 0U;
				allocatedSize = 0U;

				if (heap) {
					delete heap;
					heap = nullptr;
				}
			}

			template<typename T>
			T* allocate(const size_t count = 1) {

				const size_t size = sizeof(T) * count;
				const size_t freeSize = allocatedSize - usedSize;

				T* ptr = nullptr;
				if (size <= freeSize) {
					ptr = (T*)((char*)heap + usedSize);
				}

				return ptr;
			}

			inline void clearFrame() {
				usedSize = 0U;
			}

			size_t used() const {
				return usedSize;
			}

			size_t allocated() const {
				return allocatedSize;
			}

		private:

			size_t allocatedSize;
			size_t usedSize;

			void* heap;

		};
	}
}

#endif