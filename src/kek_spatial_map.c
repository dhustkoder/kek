#include "kek.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
static SpatialMap *maps;
static size_t mapcount = 0;
static size_t capacity = 0;


static SpatialMap *find_spatial_map(int basex, int basey);
static SpatialMap *create_spatial_map(int basex, int basey);
static void get_relative_coords(int x, int y, int *basex, int *basey, int *relx, int *rely);

void init_spatial_map(size_t cap)
{
    mapcount = 0;
    capacity = cap;
    maps = calloc(capacity, sizeof(SpatialMap));
}

void move_spatial_map_node(SpatialNode *node, int x, int y)
{
    remove_spatial_map_node(node);
    add_spatial_map_node(node, x, y);
}

void add_spatial_map_node(SpatialNode *node, int x, int y)
{
    node->x = x;
    node->y = y;

    int basex = 0;
    int basey = 0;
    int relx = 0;
    int rely = 0;

    get_relative_coords(x, y, &basex, &basey, &relx, &rely);
    SpatialMap *map = find_spatial_map(basex, basey);

    if(!map)
        map = create_spatial_map(basex, basey);

    node->next = map->nodes[rely][relx];

    map->nodes[rely][relx] = node;

}

void remove_spatial_map_node(SpatialNode *node)
{
    int basex = 0;
    int basey = 0;
    int relx = 0;
    int rely = 0;

    get_relative_coords(node->x, node->y, &basex, &basey, &relx, &rely);

    SpatialMap *map = find_spatial_map(basex, basey);

    assert(map);

    SpatialNode *iter = map->nodes[rely][relx];

    // root node
    if(map->nodes[rely][relx] == node)
    {
        map->nodes[rely][relx] = node->next;
    }
    else
    {
        while(iter)
        {
            if(iter->next == node)
            {
                iter->next = node->next;
                break;
            }
            iter = iter->next;
        }
    }
}


void query_spatial_map(int x0, int y0, int x1, int y1, SpatialMapQueryFn fn, void *ctx)
{
    for(int y = y0; y <= y1; ++y)
    {
        for(int x = x0; x <= x1; ++x)
        {
            int basex = 0;
            int basey = 0;
            int relx = 0;
            int rely = 0;

            get_relative_coords(x, y, &basex, &basey, &relx, &rely);

            SpatialMap *map = find_spatial_map(basex, basey);

            if(map)
            {
                SpatialNode *node = map->nodes[rely][relx];

                while(node)
                {
                    fn(node, ctx);
                    node = node->next;
                }
            }
        }
    }
}

static SpatialMap *create_spatial_map(int basex, int basey)
{
    assert(mapcount < capacity);
    SpatialMap *map = &maps[mapcount];
    ++mapcount;

    map->basex = basex;
    map->basey = basey;

    for(int y = 0; y < SPATIAL_NODE_YSPAN; ++y)
    {
        for(int x = 0; x < SPATIAL_NODE_XSPAN; ++x)
        {
            map->nodes[y][x] = NULL;
        }
    }
    
    return map;
}

static SpatialMap *find_spatial_map(int basex, int basey)
{
    for(int i = 0; i < mapcount; ++i)
    {
        if(maps[i].basex == basex && maps[i].basey == basey)
            return &maps[i];
    }
    
    return NULL;
}

static void get_relative_coords(int x, int y, int *basex, int *basey, int *relx, int *rely)
{
    *basex = x / SPATIAL_NODE_XSPAN;
    *basey = y / SPATIAL_NODE_YSPAN;

    if(x < 0)
        *basex -= SPATIAL_NODE_XSPAN;
    if(y < 0)
        *basey -= SPATIAL_NODE_YSPAN;

    *relx = abs(x) % SPATIAL_NODE_XSPAN;
    *rely = abs(y) % SPATIAL_NODE_YSPAN;

    if(x < 0)
        *relx = SPATIAL_NODE_XSPAN - 1 - *relx;
    if(y < 0)
        *rely = SPATIAL_NODE_YSPAN - 1 - *rely;


}
