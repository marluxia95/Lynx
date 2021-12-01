#ifndef LYNX_MEMORYMANAGER_H
#define LYNX_MEMORYMANAGER_H

#include <stdio.h>
#include "assert.h"
#include "logger.h"

#define ALLOCATOR_INITIAL_SIZE  4096
#define ALLOCATOR_BLOCK_SIZE    1024

namespace Lynx {

    class MemoryPool {
        private:
            struct MemoryBlock {
                MemoryBlock* next_block;
                void* data;
            };

        public:
            MemoryPool(size_t initial_size, size_t chunk_size) { Init(initial_size, chunk_size); }
            MemoryPool() { Init(ALLOCATOR_INITIAL_SIZE, ALLOCATOR_BLOCK_SIZE); }
            ~MemoryPool();

            void Init(size_t initial_size, size_t chunk_size);

            void* allocate(size_t size)
            {
                MemoryBlock* first_block = popLastBlock();

                LYNX_ASSERT(first_block != nullptr, "Out of memory");

                log_debug("Allocating %d bytes", size);

                for(int c = 0; c < (size % m_chunk_size)-1; c++){
                    popLastBlock();
                    m_used += m_chunk_size;
                    m_peak = std::max(m_peak, m_used);
                }

                return first_block;
            }

            void deallocate(void* ptr)
            {
                m_used -= m_chunk_size;
                pushBlock((MemoryBlock*)ptr);
            }

            size_t GetTotalMemory() { return m_total_size; }
            size_t GetUsedMemory()  { return m_used; }
            size_t GetPeakMemory()  { return m_peak; }
            size_t GetChunkSize()   { return m_chunk_size; }

            static MemoryPool* GetInstance() { return instance; }
        private:
            void resize(size_t new_size);
            void reset();
            void pushBlock(MemoryBlock* block);
            MemoryBlock* popLastBlock();

        private:
            size_t m_total_size;
            size_t m_used;
            size_t m_peak;

            size_t m_chunk_size;

            void* m_start;

            MemoryBlock* head;

            static MemoryPool* instance;

    };

    template<class T>
    class MemoryAllocator {
        public:
            typedef T value_type;

            MemoryAllocator() = default;
            template<class U> constexpr MemoryAllocator(const MemoryAllocator<U>&) noexcept {}

            [[nodiscard]] T* allocate(std::size_t size) {

                if(auto o = static_cast<T*>(MemoryPool::GetInstance()->allocate(size))) {
                    return o;
                }

                throw std::bad_alloc();
            }

            void deallocate(T* p, std::size_t size) {
                MemoryPool::GetInstance()->deallocate((void*)p);
            }
    };

}

#endif // LYNX_MEMORYMANAGER_H