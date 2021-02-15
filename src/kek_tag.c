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

void init_tag(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(TagNode));
    root = NULL;
}

uint32_t set_tag(uint32_t tag, void *value)
{
     TagNode *node = mempool_take(&pool);

     node->tag = tag;
     node->data = value;
     node->next = root;

     root = node;
}

void *get_tag(uint32_t tag)
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
void remove_tag(uint32_t tag)
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

            mempool_release(&pool, node);
        }

        prev = node;
    }
}


