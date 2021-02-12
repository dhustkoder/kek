#include "kek.h"
#include <assert.h>
#include <limits.h>

static MemPool pool;

static uint64_t get_key(SpatialMap *map, int x, int y);

static void insert_node(SpatialNode *node, int x, int y);
static void remove_node(SpatialNode *node);

void init_spatial_map(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(SpatialMap));
}

SpatialMap *create_spatial_map(SpatialNode **node_list, MemPool *node_pool, size_t xbits, size_t ybits)
{
    //todo: assertion check on node pool size
    SpatialMap *map = mem_pool_take(&pool);
    map->nodes = node_list;
    map->node_pool = node_pool;
    map->xbits = xbits;
    map->ybits = ybits;
    map->xmask = 0;
    map->ymask = 0;
    
    for(size_t i = 0; i < xbits; ++i)
        map->xmask |= 1 << i;

    for(size_t i = 0; i < ybits; ++i)
        map->ymask |= 1 << i;

    return map;
}


SpatialNode *create_spatial_node(SpatialMap *map, void *data)
{
    SpatialNode *node = mem_pool_take(map->node_pool);
    node->prev = NULL;
    node->next = NULL;
    node->data = data;
    node->map  = map;
    node->key = ~0;

    return node;
}

void destroy_spatial_node(SpatialNode *node)
{
    remove_node(node);

    mem_pool_release(node->map->node_pool, node);
}

SpatialNode *move_spatial_node(SpatialNode *node, int x, int y)
{
    remove_node(node);
    insert_node(node, x, y);
}

void query_spatial_map(SpatialMap *spatial, int x0, int y0, int x1, int y1, SpatialMapQueryFn fn, void *ctx)
{
    for(int y = y0; y <= y1; ++y)
    {
        for(int x = x0; x < x1; ++x)
        {
            uint64_t key = get_key(spatial, x, y);
            SpatialNode *node = spatial->nodes[key];

            while(node)
            {
                fn(node, ctx);
                node = node->next;
            }
        }
    }
}

static uint64_t get_key(SpatialMap *map, int x, int y)
{
    y = (y >> map->ybits) & map->ymask;
    x = (x >> map->xbits) & map->xmask;

    return (y << map->ybits) | x;
}

static void insert_node(SpatialNode *node, int x, int y)
{
    SpatialMap *map = node->map;

    uint32_t key = get_key(map, x, y);

    node->key = key;

    node->prev = NULL;
    node->next = map->nodes[key];
    
    SpatialNode *root = map->nodes[key];

    if(root)
        root->prev = node;

    map->nodes[key] = node;
}

static void remove_node(SpatialNode *node)
{
    SpatialNode *prev = node->prev;
    SpatialNode *next = node->next;
    SpatialMap *map = node->map;
    uint64_t key = node->key;
    
    if(!node->prev && !node->next)
        return;

    if(prev)
        prev->next = next;

    if(next)
        next->prev = prev;

    if(map->nodes[key] == node)
        map->nodes[key] = node->next;

    node->prev = NULL;
    node->next = NULL;
}
