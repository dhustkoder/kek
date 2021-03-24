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
    inst->id = mempool_get_slot(&pool, inst);
    inst->num_cells_x = num_cells_x;
    inst->num_cells_y = num_cells_y;
    inst->map = calloc(cellcount, sizeof(int));
    inst->texture_count = 0;
    inst->cell_size = cell_size;

    for(size_t i = 0; i < TILEMAP_INDEX_CAPACITY; ++i)
    {
        inst->indexmap[i].spritesheet = 0;
        inst->indexmap[i].uv0 = vec2(0,0);
        inst->indexmap[i].uv1 = vec2(0,0);
    }
}

void destroy_tilemap(int id)
{
    Tilemap* tilemap = mempool_get_addr(&pool, id);

    free(tilemap->map);

    mempool_release(&pool, tilemap);
}

int add_tilemap_spritesheet(int id, int texture)
{
    Tilemap *tilemap = mempool_get_addr(&pool, id);

    int count = tilemap->texture_count;
    assert(count < TILEMAP_SPRITESHEET_CAPACITY);
    
    tilemap->textures[count] = texture;

    ++count;

    tilemap->texture_count = count;
}

void bind_tilemap_index(int id, int index, int spritesheet, Vec2 uv0, Vec2 uv1)
{
    Tilemap *tilemap = mempool_get_addr(&pool, id);

    tilemap->indexmap[index].spritesheet = spritesheet;
    tilemap->indexmap[index].uv0 = uv0;
    tilemap->indexmap[index].uv1 = uv1;
}

void tilemap_cell_index(int id, int cellx, int celly, int spritesheet, int index)
{
    assert(cellx >= 0);
    assert(celly >= 0);
    assert(cellx < tilemap->num_cells_x);
    assert(celly < tilemap->num_cells_y);

    Tilemap *tilemap = mempool_get_addr(&pool, id);

    int offset = celly * tilemap->num_cells_x + cellx;

    tilemap->map[offset] = index;
}
