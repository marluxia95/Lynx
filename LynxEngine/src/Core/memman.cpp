#include <stdlib.h>
#include "memman.h"


namespace Lynx {
    MemoryPool* MemoryPool::instance = nullptr;

    void MemoryPool::Init(size_t initial_size, size_t chunk_size)
    {
        m_total_size = initial_size;
        m_chunk_size = chunk_size;
        instance = this;

        log_debug("Initializing allocator with size %d and chunk size %d", initial_size, chunk_size);

        m_start = malloc(initial_size);
        
        reset();

    }

    MemoryPool::~MemoryPool()
    {
        log_debug("Deallocating memory");
        free(m_start);
    }

    void MemoryPool::reset()
    {
        m_used = 0;
        m_peak = 0;
        const int chunks = m_total_size / m_chunk_size;
        log_debug("Creating %d chunks", chunks);

        for ( int b = 0; b < chunks; b++ ) {
            size_t addr = (size_t)m_start + b*m_chunk_size;
            log_debug("Pushing block 0x%x", addr);
            pushBlock((MemoryBlock*)addr);
        }
    }

    void MemoryPool::pushBlock(MemoryBlock* block)
    {
        block->next_block = head;
        head = block;
    }

    MemoryPool::MemoryBlock* MemoryPool::popLastBlock()
    {
        MemoryBlock* block = head;
        head = head->next_block;
        return block;
    }

}