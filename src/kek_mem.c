#include "kek.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

static uint8_t *index_to_addr(struct mem_pool *pool, size_t index)
{
    size_t offset = index * pool->stride;
    return &pool->buffer[offset];
}

static int32_t addr_to_index(struct mem_pool *pool, uint8_t *addr)
{
    uint8_t *start = pool->buffer;
    size_t offset = (size_t)(addr - start);

    return (int32_t)(offset / pool->stride);
}

void mem_pool_alloc(struct mem_pool *pool, size_t capacity, size_t stride)
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
void mem_pool_free(struct mem_pool *pool)
{
    free(pool->buffer);

    pool->capacity = 0;
    pool->stride = 0;
    pool->free_head = 0;
    pool->free_count = 0;
    pool->use_count = 0;
}

void *mem_pool_take(struct mem_pool *pool)
{
    if(pool->free_count == 0)
        return NULL;

    int32_t *nextp = (int32_t *)index_to_addr(pool, pool->free_head);

    pool->free_head = *nextp;
    pool->free_count--;
    pool->use_count++;

    return nextp;
}


void *mem_pool_release(struct mem_pool *pool, void *addr)
{
   size_t *head = (size_t *)addr;
   *head = pool->free_head;

    pool->free_head = addr_to_index(pool, (uint8_t *)addr);

    pool->free_count++; 
    pool->use_count--;
}

struct stack_header {
    void *addr;
    int32_t size;
};

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
    if(stack_size + size + sizeof(struct stack_header)  > stack_capacity)
        return NULL;

    uint8_t *addr = &stack_buffer[stack_size];

    struct stack_header header;
    header.addr = addr;
    header.size = size;

    stack_size += size;
    memcpy(&stack_buffer[stack_size], &header, sizeof(header));
    stack_size += sizeof(header);

    return addr;
}

void mem_stack_pop(void *addr)
{
    assert(stack_size >= sizeof(struct stack_header));

    struct stack_header *header = (struct stack_header *)&stack_buffer[stack_size - sizeof(struct stack_header)];
    
    assert(header->addr == addr);
    stack_size -= sizeof(struct stack_header) + header->size;
}

void mem_stack_free(void)
{
    free(stack_buffer);
    stack_buffer = NULL;
}
