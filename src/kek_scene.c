#include "kek.h"

static struct mem_pool pool;
static struct mem_pool pool_entities;

static struct scene *active_scene = NULL;
static struct camera default_camera = {0,0,0.9};
void scene_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(struct scene));
    mem_pool_alloc(&pool, capacity, sizeof(struct scene));
}

struct scene *scene_create(void)
{
     struct scene *scene = mem_pool_take(&pool);

     scene->entities = NULL;
     scene->entity_count = 0;
     scene->render_default = render_create();
     scene->camera = &default_camera;

     return scene;
}

void scene_destroy(struct scene *scene)
{
    render_destroy(scene->render_default);

    if(scene == active_scene)
        active_scene = NULL;
    
    mem_pool_release(&pool, scene);
}

void scene_set_active(struct scene *scene)
{
    active_scene = scene;
}

struct scene *scene_get_active(void)
{
    return active_scene;
}


void scene_set_camera(struct scene *scene, struct camera *camera)
{
    scene->camera = camera;
}

struct camera *scene_get_camera(struct scene *scene)
{
    return scene->camera;
}

void scene_add_entity(struct scene *scene, struct entity *entity)
{
    entity->scene_next_entity = scene->entities;
    scene->entities = entity;
    scene->entity_count++;
}

void scene_update(struct scene *scene)
{
    struct entity *entity = scene->entities;

    while(entity)
    {
        entity_update(entity);

        entity = entity->scene_next_entity;
    }
}

void scene_garbage_collect(struct scene *scene)
{
    struct entity *entity = scene->entities;
    struct entity *prev = NULL;

    while(entity)
    {
        struct entity *next = entity->scene_next_entity;

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

void scene_draw(struct scene *scene)
{
    // cull all objects
    // sort objects
    // order by material
    // write to vbos and keep track of each Draw
    struct entity *entity = scene->entities;

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
void scene_query_entities_aabb(struct scene *scene, union vec2 pos, union vec2 size, KEKSceneQueryEntityFn fn, void *ctx)
{
    struct entity *entity = scene->entities;

    while(entity)
    {
        if(aabb(pos, size, entity->position.xy, entity->size.xy))
            fn(entity, ctx);

        entity = entity->scene_next_entity;
    }
}
