#include "kek.h"
#include <string.h>


struct tag_node
{
    uint32_t tag;
    void *data;
    struct tag_node *next;
};

static struct mem_pool pool;
static struct tag_node *root = NULL;

void tag_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(struct tag_node));
    root = NULL;
}

uint32_t tag_set(uint32_t tag, void *value)
{
     struct tag_node *node = mem_pool_take(&pool);

     node->tag = tag;
     node->data = value;
     node->next = root;

     root = node;
}

void *tag_get(uint32_t tag)
{
    struct tag_node *node = root;

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
    struct tag_node *node = root;
    struct tag_node *prev = NULL;

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


