#include "kek.h"
#include <assert.h>
#include <stdlib.h>

static MemPool pool;

void init_tilemap(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Tilemap));
}

int create_tilemap(int num_cells_x, int num_cells_y, Vec2 cell_size)
{
    int cellcount = num_cells_x * num_cells_y;

    Tilemap *inst = mempool_take(&pool);

    inst->id            = mempool_get_slot(&pool, inst);
    inst->num_cells_x   = num_cells_x;
    inst->num_cells_y   = num_cells_y;
    inst->map           = calloc(cellcount, sizeof(int));
    inst->texture_count = 0;
    inst->cell_size     = cell_size;
    inst->scene_next_tilemap = NULL;
    inst->layer         = 0;

    for(size_t i = 0; i < TILEMAP_INDEX_CAPACITY; ++i)
    {
        inst->indexmap[i].spritesheet = 0;
        inst->indexmap[i].uv0 = vec2(0,0);
        inst->indexmap[i].uv1 = vec2(1,0);
        inst->indexmap[i].flags = 0;
    }

    return inst->id;
}

void destroy_tilemap(int id)
{
    Tilemap* tilemap = mempool_get_addr(&pool, id);

    free(tilemap->map);

    mempool_release(&pool, tilemap);
}

TilemapSpritesheetClip get_tilemap_cell_clip(int id, int cellx, int celly)
{
    Tilemap *tilemap = mempool_get_addr(&pool, id);

    assert(cellx >= 0);
    assert(celly >= 0);
    assert(cellx < tilemap->num_cells_x);
    assert(celly < tilemap->num_cells_y);

    int offset = celly * tilemap->num_cells_x + cellx;

    int index = tilemap->map[offset];

    return tilemap->indexmap[index];
}

int add_tilemap_spritesheet(int id, int texture)
{
    Tilemap *tilemap = mempool_get_addr(&pool, id);

    int index = tilemap->texture_count;
    assert(index < TILEMAP_SPRITESHEET_CAPACITY);
    
    tilemap->textures[index] = texture;

    tilemap->texture_count++;

    return index;
}

void bind_tilemap_index(int id, int index, int spritesheet, int x, int y, int clip_width, int clip_height, uint32_t flags)
{
    Tilemap *tilemap = mempool_get_addr(&pool, id);
    Texture *texture = get_texture(spritesheet);

    y = texture->height - y - clip_height;

    tilemap->indexmap[index].spritesheet = spritesheet;
    tilemap->indexmap[index].uv0.x = (float)x/(float)texture->width;
    tilemap->indexmap[index].uv0.y = (float)y/(float)texture->height;
    tilemap->indexmap[index].uv1.x = (float)(x + clip_width)/(float)texture->width;
    tilemap->indexmap[index].uv1.y = (float)(y + clip_height)/(float)texture->height;
    tilemap->indexmap[index].flags = flags;

}

Tilemap *get_tilemap(int id)
{
    return mempool_get_addr(&pool, id);
}

void tilemap_cell_index(int id, int cellx, int celly, int index)
{
    Tilemap *tilemap = mempool_get_addr(&pool, id);

    assert(cellx >= 0);
    assert(celly >= 0);
    assert(cellx < tilemap->num_cells_x);
    assert(celly < tilemap->num_cells_y);

    int offset = celly * tilemap->num_cells_x + cellx;

    tilemap->map[offset] = index;
}

void tilemap_layer(int id, int layer)
{
    Tilemap *tilemap = get_tilemap(id);
    tilemap->layer = layer;
}

int get_tilemap_layer(int id)
{
    Tilemap *tilemap = get_tilemap(id);
    return tilemap->layer;
}

uint32_t get_tile_flags(int tilemapid, int x, int y)
{
    Tilemap *tilemap = get_tilemap(tilemapid);
    int cellx = (int)((float)x / tilemap->cell_size.x);
    int celly = (int)((float)y / tilemap->cell_size.y);

    if(cellx < 0) return 0;
    if(celly < 0) return 0;
    if(cellx >= tilemap->num_cells_x) return 0;
    if(celly >= tilemap->num_cells_y) return 0;

    int offset = celly * tilemap->num_cells_x + cellx;

    int index = tilemap->map[offset];

    return tilemap->indexmap[index].flags;
}
