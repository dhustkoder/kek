#include "kek.h"
#include <assert.h>

static MemPool pool;
static MemPool pool_entities;

static Scene *active_scene = NULL;
static Camera default_camera = {0,0,0.9};
void init_scene(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Scene));
}

Scene *create_scene(void)
{
     Scene *scene = mempool_take(&pool);

     scene->entities = NULL;
     scene->entity_count = 0;
     scene->render_entity = create_entity_render();
     scene->render_entity_box = create_entity_box_render();
     scene->render_spatialmap = create_entity_box_render();
     scene->camera = &default_camera;

     return scene;
}

void destroy_scene(Scene *scene)
{
    destroy_render(scene->render_entity);
    destroy_render(scene->render_entity_box);

    if(scene == active_scene)
        active_scene = NULL;
    
    mempool_release(&pool, scene);
}

void scene_active(Scene *scene)
{
    active_scene = scene;
}

Scene *get_active_scene(void)
{
    return active_scene;
}


void scene_camera(Scene *scene, Camera *camera)
{
    scene->camera = camera;
}

Camera *get_scene_camera(Scene *scene)
{
    return scene->camera;
}

void add_scene_entity(Scene *scene, Entity *entity)
{
    entity->scene_next_entity = scene->entities;
    scene->entities = entity;
    scene->entity_count++;
}

void update_scene(Scene *scene)
{
    Entity *entity = scene->entities;

    while(entity)
    {
        update_entity(entity);

        entity = entity->scene_next_entity;
    }
}

void garbage_collect_scene(Scene *scene)
{
    Entity *entity = scene->entities;
    Entity *prev = NULL;

    while(entity)
    {
        Entity *next = entity->scene_next_entity;

        if(entity->destroy)
        {
            assert(scene->entity_count > 0);
            scene->entity_count--;
            if(prev)
                prev->scene_next_entity = next;
            else
                scene->entities = next;

            destroy_entity(entity);
        }
        else
        {
            prev = entity;
        }

        entity = next;
    }
}


static uint32_t get_entity_render_key(Entity *e)
{
    uint32_t type = e->type;
    uint32_t texture = e->texture;
    AnimationFrame *frame = get_entity_animation_frame(e);

    if(frame)
        texture = frame->texture;

    uint32_t key = (type << 16) | (texture & 0xFFFF);

    return key;
}

void draw_scene(Scene *scene)
{
    Entity *entity = scene->entities;

    int window_width;
    int window_height;

    get_window_size(&window_width, &window_height);

    entity = scene->entities;

    size_t listcount = scene->entity_count;
    Entity **sortlist = memstack_push(listcount * sizeof(Entity *));
    entity = scene->entities;

    for(int i = 0; i < listcount; ++i)
    {
        assert(entity);
        sortlist[i] = entity;
        entity = entity->scene_next_entity;
    }

    // bubble sort, yea, yeah, i know...
    for(int i = 0; i < listcount; ++i)
    {
        for(int j = i + 1; j < listcount; ++j)
        {
            Entity *a = sortlist[i];
            Entity *b = sortlist[j];

            uint32_t akey = get_entity_render_key(a);
            uint32_t bkey = get_entity_render_key(b);

            if(bkey < akey)
            {
                sortlist[i] = b;
                sortlist[j] = a;
            }
        }
    }

    draw_render_entities(scene->render_entity, scene->camera, sortlist, listcount, NULL);
    draw_render_entity_boxes(scene->render_entity_box, scene->camera, sortlist, listcount, NULL);
    draw_render_spatialmap(scene->render_spatialmap, scene->camera, sortlist, listcount);
    
    memstack_pop(sortlist);
}

void query_scene_entities_aabb(Scene *scene, Vec2 pos, Vec2 size, SceneQueryEntityFn fn, void *ctx)
{
    Entity *entity = scene->entities;

    while(entity)
    {
        if(aabb(pos, size, entity->position.xy, entity->size.xy))
            fn(entity, ctx);

        entity = entity->scene_next_entity;
    }
}
