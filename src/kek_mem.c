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

void mem_pool_alloc(MemPool *pool, size_t capacity, size_t stride)
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
        *addr = i + 1;
    }
}
void mem_pool_free(MemPool *pool)
{
    free(pool->buffer);

    pool->capacity = 0;
    pool->stride = 0;
    pool->free_head = 0;
    pool->free_count = 0;
    pool->use_count = 0;
}

void *mem_pool_take(MemPool *pool)
{
    if(pool->free_count == 0)
        return NULL;

    int32_t *nextp = (int32_t *)index_to_addr(pool, pool->free_head);

    pool->free_head = *nextp;
    pool->free_count--;
    pool->use_count++;

    return nextp;
}


void *mem_pool_release(MemPool *pool, void *addr)
{
   size_t *head = (size_t *)addr;
   *head = pool->free_head;

    pool->free_head = addr_to_index(pool, (uint8_t *)addr);

    pool->free_count++; 
    pool->use_count--;
}

static uint8_t *stack_buffer = NULL;
static size_t stack_capacity = 0;
static size_t stack_size = 0;

void  mem_stack_init(size_t capacity)
{
    stack_buffer = calloc(capacity, sizeof(uint8_t));
    stack_capacity = capacity;
    stack_size = 0;
}

void *mem_stack_push(size_t size)
{
    if(stack_size + size  > stack_capacity)
        return NULL;

    uint8_t *addr = &stack_buffer[stack_size];

    stack_size += size;

    return addr;
}

void mem_stack_pop(void *addr)
{
    uint8_t *addr8 = addr;
    assert(addr8 >= stack_buffer);
    assert(addr8 <= &stack_buffer[stack_size]);


    stack_size = addr8 - stack_buffer;
}

void mem_stack_free(void)
{
    free(stack_buffer);
    stack_buffer = NULL;
}

size_t mem_stack_size(void)
{
    return stack_size;
}

size_t mem_stack_capacity(void)
{
    return stack_capacity;
}
