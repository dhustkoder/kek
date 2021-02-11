#include "kek.h"
#include <stdlib.h>

#define SPATIAL_MAP_RESOLUTION_BITS 8 
#define WORLD_TO_SPATIAL_CELL(x) (x / 128)
// each cell is 128 physical units long
// there are 2^8 cells wide and 2^8 cells high
// 2^8 * 128 = 32768 is the maximum position allowed on each axis

typedef struct entity_callbacks EntityCallbacks;

typedef struct entity_callbacks {
    uint32_t type;
    EntityTerminateFn terminate;
    EntityUpdateFn update;
    void *ctx;
    EntityCallbacks *next;
} EntityCallbacks;

static MemPool pool_entity;
static MemPool pool_callbacks;
static MemPool pool_snode;
static SpatialMap *smap = NULL;
static EntityCallbacks *root_callbacks = NULL;
static SpatialNode **spatial_node_list = NULL;

static EntityCallbacks *entity_getinsert_callbacks(uint32_t type);

void entity_init(size_t capacity, size_t type_capacity, size_t user_data_stride)
{
    mem_pool_alloc(&pool_entity, capacity, sizeof(Entity) + user_data_stride);
    mem_pool_alloc(&pool_callbacks, type_capacity, sizeof(EntityCallbacks));
    mem_pool_alloc(&pool_snode, capacity * 2, sizeof(SpatialNode)); // *2 is just for added padding 
    size_t map_capacity = (1 << SPATIAL_MAP_RESOLUTION_BITS);
    map_capacity *= map_capacity;
    spatial_node_list = calloc(map_capacity * 2, sizeof(SpatialNode *));


    // create a 256x256 spatialmap  
    smap = spatialmap_create(spatial_node_list, &pool_snode, SPATIAL_MAP_RESOLUTION_BITS, SPATIAL_MAP_RESOLUTION_BITS);

    root_callbacks = NULL;
}

Entity *entity_create(uint32_t type)
{
     Entity *inst = mem_pool_take(&pool_entity);

    if(inst == NULL)
        return NULL;

    inst->destroy = false;
    inst->type = type;
    inst->scene_next_entity = NULL;
    inst->velocity = vec3_zero();
    inst->rotation = vec3_zero();
    inst->size     = vec3(128, 128, 128);

    inst->colormask = vec4(1,1,1,1);
    inst->texture = NULL;
    inst->animation = NULL;
    inst->animation_frame = 0;
    inst->animation_speed = 1.0f;
    inst->animation_frame_time = 0.0f;

    inst->position = vec3_zero();
    inst->snode = spatialmap_create_node(smap, inst);

    spatialmap_move_node(inst->snode, 0, 0);

    return inst;
}

void entity_destroy(Entity *entity)
{
    spatialmap_destroy_node(entity->snode);
    mem_pool_release(&pool_entity, entity);
}

void entity_release(Entity *entity)
{
    entity->destroy = true;
}

typedef struct {
    EntityQueryFn fn;
    void *ctx;
}EntityQueryData ;

void query_spatialmap_cb(SpatialNode *node, void *ctx)
{
    EntityQueryData *data = ctx;

    data->fn(node->data, data->ctx);
}

void entity_query(Vec2 p0, Vec2 p1, EntityQueryFn fn, void *ctx)
{
    EntityQueryData data = {fn, ctx};
    int x0 = WORLD_TO_SPATIAL_CELL(p0.x);
    int y0 = WORLD_TO_SPATIAL_CELL(p0.y);
    int x1 = WORLD_TO_SPATIAL_CELL(p1.x);
    int y1 = WORLD_TO_SPATIAL_CELL(p1.y);
    spatialmap_query(smap, x0, y0, x1, y1, query_spatialmap_cb, &data);
}

void entity_update(Entity *e)
{
    EntityCallbacks *cb = entity_getinsert_callbacks(e->type);
    if(cb && cb->update)
    {
        // update the animation
        if(e->animation && e->animation->frame_count > 0)
        {
            unsigned int idx = e->animation_frame;
            Animation *anim = e->animation;
            AnimationFrame *frame = &anim->frames[idx];

            // safety to prevent bad timer and being in an infinite loop
            unsigned int counter = 0;
            while((e->animation_frame_time > frame->duration) && (counter < anim->frame_count))
            {
                e->animation_frame_time -= anim->frames[idx].duration * e->animation_speed;
                
                if(idx < anim->frame_count - 1)
                    ++idx;
                else if(anim->loop)
                    idx = 0;
                else
                    break;

                ++counter;
            }

            e->animation_frame = idx;
            e->animation_frame_time += kek_get_target_frame_interval() * e->animation_speed;
        }

        cb->update(e, cb->ctx);
    }
}

void entity_set_update_callback(uint32_t type, EntityUpdateFn callback)
{
    EntityCallbacks *cb = entity_getinsert_callbacks(type);
    cb->update = callback;
}

void entity_set_terminate_callback(uint32_t type, EntityTerminateFn callback)
{
    EntityCallbacks *cb = entity_getinsert_callbacks(type);
    cb->terminate = callback;
}

void entity_set_callback_context(uint32_t type, void *ctx)
{
    EntityCallbacks *cb = entity_getinsert_callbacks(type);
    cb->ctx = ctx;
}

void *entity_get_user_data(Entity *entity)
{
    uint8_t *addr = (uint8_t *)entity;

    addr += sizeof(Entity);

    return addr;
}

static EntityCallbacks *entity_getinsert_callbacks(uint32_t type)
{
    EntityCallbacks *cb = root_callbacks;

    while(cb)
    {
        if(cb->type == type)
            return cb;

        cb = cb->next;
    }

    // if we reach this point, the callback does not exist make a new instance
    // and attach it to the root 
    cb = mem_pool_take(&pool_callbacks);

    cb->type = type;
    cb->terminate = NULL;
    cb->update = NULL;
    cb->ctx = NULL;

    cb->next = root_callbacks;
    root_callbacks = cb;

    return cb;
}

AnimationFrame *entity_get_animation_frame(Entity *e)
{
    if(!e->animation)
        return NULL;

    if(e->animation->frame_count == 0)
        return NULL;

    if(e->animation->frame_count <= e->animation_frame)
        return NULL;

    return &e->animation->frames[e->animation_frame];
}

void entity_set_animation(Entity *e, Animation *animation)
{
    e->animation = animation;

    entity_reset_animation(e);
}

Vec3 entity_size(Entity *e)
{
    return e->size;
}

void entity_set_size(Entity *e, Vec3 size)
{
    e->size = size;
}

Vec3 entity_position(Entity *e)
{
    return e->position;
}

Vec3 entity_velocity(Entity *e)
{
    return e->velocity;
}

void entity_set_position(Entity *e, Vec3 position)
{
    e->position = position;
    int x = WORLD_TO_SPATIAL_CELL(position.x);
    int y = WORLD_TO_SPATIAL_CELL(position.y);
    spatialmap_move_node(e->snode, x, y);
}

void entity_set_velocity(Entity *e, Vec3 velocity)
{
    e->velocity = velocity;
}

void entity_set_rotation(Entity *e, Vec3 rotation)
{
    e->rotation = rotation;
}

void entity_set_texture(Entity *e, Texture *texture)
{
    e->texture = texture;
}

void entity_set_rotation_z(Entity *e, float rotation)
{
    e->rotation.z = rotation;
}

void entity_reset_animation(Entity *e)
{
    e->animation_frame = 0;
    e->animation_frame_time = 0.0f;
}

void entity_set_animation_speed(Entity *e, float speed)
{
    e->animation_speed = speed;
}
