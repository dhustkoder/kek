#include "kek.h"

static MemPool pool;
static MemPool pool_entities;

static Scene *active_scene = NULL;
static Camera default_camera = {0,0,0.9};
void scene_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(Scene));
    mem_pool_alloc(&pool, capacity, sizeof(Scene));
}

Scene *scene_create(void)
{
     Scene *scene = mem_pool_take(&pool);

     scene->entities = NULL;
     scene->entity_count = 0;
     scene->render_default = render_create();
     scene->camera = &default_camera;

     return scene;
}

void scene_destroy(Scene *scene)
{
    render_destroy(scene->render_default);

    if(scene == active_scene)
        active_scene = NULL;
    
    mem_pool_release(&pool, scene);
}

void scene_set_active(Scene *scene)
{
    active_scene = scene;
}

Scene *scene_get_active(void)
{
    return active_scene;
}


void scene_set_camera(Scene *scene, Camera *camera)
{
    scene->camera = camera;
}

Camera *scene_get_camera(Scene *scene)
{
    return scene->camera;
}

void scene_add_entity(Scene *scene, Entity *entity)
{
    entity->scene_next_entity = scene->entities;
    scene->entities = entity;
    scene->entity_count++;
}

void scene_update(Scene *scene)
{
    Entity *entity = scene->entities;

    while(entity)
    {
        entity_update(entity);

        entity = entity->scene_next_entity;
    }
}

void scene_garbage_collect(Scene *scene)
{
    Entity *entity = scene->entities;
    Entity *prev = NULL;

    while(entity)
    {
        Entity *next = entity->scene_next_entity;

        if(entity->destroy)
        {
            if(prev)
                prev->scene_next_entity = next;
            else
                scene->entities = next;

            entity_destroy(entity);
        }
        else
        {
            prev = entity;
        }

        entity = next;
    }
}

void scene_draw(Scene *scene)
{
    // cull all objects
    // sort objects
    // order by material
    // write to vbos and keep track of each Draw
    Entity *entity = scene->entities;

    int window_width;
    int window_height;
    window_get_size(&window_width, &window_height);

    size_t vertices = 0;

    int maxid = 0;

    entity = scene->entities;
    while(entity)
    {
        maxid = KUT_MAX(maxid, entity->type);
        entity = entity->scene_next_entity;
    }

    // slow hack
    for(int i = 0; i <= maxid; ++i)
    {
        entity = scene->entities;
        while(entity)
        {
            if(entity->type == i)
                render_draw(scene->render_default, scene->camera, entity);

            entity = entity->scene_next_entity;
        }
    }
}
void scene_query_entities_aabb(Scene *scene, Vec2 pos, Vec2 size, KEKSceneQueryEntityFn fn, void *ctx)
{
    Entity *entity = scene->entities;

    while(entity)
    {
        if(aabb(pos, size, entity->position.xy, entity->size.xy))
            fn(entity, ctx);

        entity = entity->scene_next_entity;
    }
}
