#include "kek.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

static uint8_t *stack_buffer = NULL;
static size_t stack_capacity = 0;
static size_t stack_size = 0;

void  memstack_init(size_t capacity)
{
    stack_buffer = calloc(capacity, sizeof(uint8_t));
    stack_capacity = capacity;
    stack_size = 0;
}

void *memstack_push(size_t size)
{
    if(stack_size + size  > stack_capacity)
        return NULL;

    uint8_t *addr = &stack_buffer[stack_size];

    stack_size += size;

    return addr;
}

void memstack_pop(void *addr)
{
    uint8_t *addr8 = addr;
    assert(addr8 >= stack_buffer);
    assert(addr8 <= &stack_buffer[stack_size]);


    stack_size = addr8 - stack_buffer;
}

void memstack_free(void)
{
    free(stack_buffer);
    stack_buffer = NULL;
}

size_t memstack_size(void)
{
    return stack_size;
}

size_t memstack_capacity(void)
{
    return stack_capacity;
}
