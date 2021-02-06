#include "kek.h"
#include <string.h>

typedef struct tag_node TagNode;

typedef struct tag_node
{
    uint32_t tag;
    void *data;
    TagNode *next;
} TagNode;

static MemPool pool;
static TagNode *root = NULL;

void tag_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(TagNode));
    root = NULL;
}

uint32_t tag_set(uint32_t tag, void *value)
{
     TagNode *node = mem_pool_take(&pool);

     node->tag = tag;
     node->data = value;
     node->next = root;

     root = node;
}

void *tag_get(uint32_t tag)
{
    TagNode *node = root;

    while(node)
    {
        if(node->tag == tag)
            return node->data;
        
        node = node->next;
    }
    
    loge("Tag '%s' does not exist", tag);

    return NULL;
}
void tag_remove(uint32_t tag)
{
    TagNode *node = root;
    TagNode *prev = NULL;

    while(node)
    {
        if(node->tag == tag)
        {
            if(prev)
                prev->next = node->next;

            if(node == root)
                root = node->next;

            mem_pool_release(&pool, node);
        }

        prev = node;
    }
}


