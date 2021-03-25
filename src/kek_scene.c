#include "kek.h"
#include <assert.h>
#include <stdlib.h>

static MemPool pool;
static MemPool pool_entities;

static int active_scene = -1;
static int default_camera = -1;
static int sortlist_sort(const void *a, const void *b);

void init_scene(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Scene));
}

int create_scene(void)
{
     Scene *scene = mempool_take(&pool);

     scene->id = mempool_get_slot(&pool, scene);
     scene->entities = NULL;
     scene->entity_count = 0;
     scene->tilemaps = NULL;
     scene->tilemap_count = 0;
     scene->render_entity = create_entity_render();
     scene->render_tilemap = create_entity_render();
#if 0
     scene->render_spatialmap = create_entity_box_render();
     scene->render_entity_box = create_entity_box_render();
     scene->render_rect = create_rect_render();
     scene->render_circle = create_circle_render();
#endif
    
     if(default_camera == -1)
        default_camera = create_camera();

     scene->camera = default_camera;

     return scene->id;
}

Scene *get_scene(int sceneid)
{
    return mempool_get_addr(&pool, sceneid);
}

void destroy_scene(int sceneid)
{
    Scene *scene = get_scene(sceneid);
    destroy_render(scene->render_entity);
    //destroy_render(scene->render_entity_box);

    if(sceneid == active_scene)
        active_scene = -1;
    
    mempool_release(&pool, scene);
}

void scene_active(int sceneid)
{
    active_scene = sceneid;
}

int get_active_scene(void)
{
    return active_scene;
}


void scene_camera(int sceneid, int camera)
{
    Scene *scene = get_scene(sceneid);
    scene->camera = camera;
}

int get_scene_camera(int sceneid)
{
    Scene *scene = get_scene(sceneid);
    return scene->camera;
}

void add_scene_entity(int sceneid, int entityid)
{
    Scene *scene = get_scene(sceneid);
    Entity *entity = get_entity(entityid);
    entity->scene_next_entity = scene->entities;
    scene->entities = entity;
    scene->entity_count++;
}

void add_scene_tilemap(int sceneid, int tilemapid)
{
    Scene *scene = get_scene(sceneid);
    Tilemap *tilemap = get_tilemap(tilemapid);
    tilemap->scene_next_tilemap = scene->tilemaps;
    scene->tilemaps = tilemap;
    scene->tilemap_count++;
}

void update_scene(int sceneid)
{
    Scene *scene = get_scene(sceneid);
    Entity *entity = scene->entities;

    simulate_physics(sceneid);

    while(entity)
    {
        update_entity(entity->id);

        entity = entity->scene_next_entity;
    }

    update_camera(scene->camera);
}

void garbage_collect_scene(int sceneid)
{
    Scene *scene = get_scene(sceneid);
    Entity *entity = scene->entities;
    Entity *prev = NULL;

    while(entity)
    {
        Entity *next = entity->scene_next_entity;

#warning MUST HANDLE TILEMAP DELETIONS HERE AS WELL
        if(entity->destroy)
        {
            assert(scene->entity_count > 0);
            scene->entity_count--;
            if(prev)
                prev->scene_next_entity = next;
            else
                scene->entities = next;

            destroy_entity(entity->id);
        }
        else
        {
            prev = entity;
        }

        entity = next;
    }
}


void draw_scene(int sceneid)
{
    Scene *scene = get_scene(sceneid);
    Entity *entity = scene->entities;

    int window_width;
    int window_height;

    get_window_size(&window_width, &window_height);

    // Submit all culled entities to the render queue
    entity = scene->entities;

    while(entity)
    {
        draw_submit_entity(scene->render_entity, entity->id, scene->camera);

        entity = entity->scene_next_entity;
    }

    // Submit all tilemaps to the render queue
    Tilemap *tilemap = scene->tilemaps;
    while(tilemap)
    {
        draw_submit_tilemap(scene->render_tilemap, tilemap->id, scene->camera);

        tilemap = tilemap->scene_next_tilemap;
    }
    draw();
}

static int sortlist_sort(const void *a, const void *b)
{
    uint32_t akey = get_entity_render_key(*(int *)a);
    uint32_t bkey = get_entity_render_key(*(int *)b);
    if(akey > bkey)
        return 1;
    else if(akey < bkey)
        return -1;

    return 0;
}

