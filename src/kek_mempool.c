#include "kek.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

static uint8_t *index_to_addr(MemPool *pool, size_t index)
{
    size_t offset = index * pool->stride;
    return &pool->buffer[offset];
}

static int32_t addr_to_index(MemPool *pool, uint8_t *addr)
{
    uint8_t *start = pool->buffer;
    size_t offset = (size_t)(addr - start);

    return (int32_t)(offset / pool->stride);
}

int mempool_get_slot(MemPool *pool, void *addr)
{
    return addr_to_index(pool, addr);
}

void *mempool_get_addr(MemPool *pool, int slot)
{
    return index_to_addr(pool, slot);
}

void mempool_alloc(MemPool *pool, size_t capacity, size_t stride)
{
    // assume each slot is 8 large,
    // for the free, use as a next offset
    assert(stride >= sizeof(int32_t));
    assert(stride * capacity < INT_MAX);
    assert(stride % 4 == 0); 

    pool->buffer = (uint8_t *)calloc(capacity, stride);
    pool->capacity = capacity;
    pool->stride = stride;
    pool->free_head = 0;
    pool->free_count = capacity;
    pool->use_count = 0;

    // use each unused node as a next pointer
    for(size_t i = 0; i < capacity; ++i)
    {
        uint32_t *addr = (uint32_t *)index_to_addr(pool, i);
        *addr = (uint32_t)i + 1;
    }
}
void mempool_free(MemPool *pool)
{
    free(pool->buffer);

    pool->capacity = 0;
    pool->stride = 0;
    pool->free_head = 0;
    pool->free_count = 0;
    pool->use_count = 0;
}

void *mempool_take(MemPool *pool)
{
    if(pool->free_count == 0)
        return NULL;

    int32_t *nextp = (int32_t *)index_to_addr(pool, pool->free_head);

    pool->free_head = *nextp;
    pool->free_count--;
    pool->use_count++;

    return nextp;
}


void mempool_release(MemPool *pool, void *addr)
{
   size_t *head = (size_t *)addr;
   *head = pool->free_head;

    pool->free_head = addr_to_index(pool, (uint8_t *)addr);

    pool->free_count++; 
    pool->use_count--;
}
