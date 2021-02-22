#include "kek.h"
#include <stdlib.h>

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
static EntityCallbacks *root_callbacks = NULL;

static EntityCallbacks *getinsert_entity_callbacks(uint32_t type);

void init_entity(size_t capacity, size_t type_capacity, size_t user_data_stride)
{
    mempool_alloc(&pool_entity, capacity, sizeof(Entity) + user_data_stride);
    mempool_alloc(&pool_callbacks, type_capacity, sizeof(EntityCallbacks));

    root_callbacks = NULL;
}

Entity *get_entity(int entityid)
{
    return mempool_get_addr(&pool_entity, entityid);
}

int create_entity(uint32_t type)
{
    Entity *inst = mempool_take(&pool_entity);

    inst->id = mempool_get_slot(&pool_entity, inst);
    inst->destroy = false;
    inst->type = type;
    inst->scene_next_entity = NULL;
    inst->velocity = zero_vec3();
    inst->rotation = zero_vec3();
    inst->size     = vec3(128, 128, 128);

    inst->collider.type = COLLIDER_NONE;
    inst->collider.mask = 0;
    inst->collider.category = 0;
    inst->collider.collision_fn = NULL;
    inst->collider.ctx = NULL;

    inst->colormask = vec4(1,1,1,1);
    inst->texture = 0;
    inst->animation = NULL;
    inst->animation_frame = 0;
    inst->animation_speed = 1.0f;
    inst->animation_frame_time = 0.0f;
    inst->snode.data = inst;

    inst->position = zero_vec3();

    int x = WORLD_TO_SPATIAL_CELL((int)inst->position.x);
    int y = WORLD_TO_SPATIAL_CELL((int)inst->position.y);
    add_spatialmap_node(&inst->snode, x, y);

    return inst->id;
}

void destroy_entity(int entityid)
{
    Entity *entity = get_entity(entityid);
    remove_spatialmap_node(&entity->snode);
    mempool_release(&pool_entity, entity);
}

void release_entity(int entityid)
{
    Entity *entity = get_entity(entityid);
    entity->destroy = true;
}

typedef struct {
    EntityQueryFn fn;
    void *ctx;
}EntityQueryData ;

void query_spatial_cb(SpatialNode *node, void *ctx)
{
    EntityQueryData *data = ctx;
    Entity *entity = node->data;

    data->fn(entity->id, data->ctx);
}

void query_entity(Vec2 p0, Vec2 p1, EntityQueryFn fn, void *ctx)
{
    EntityQueryData data = {fn, ctx};
    int x0 = WORLD_TO_SPATIAL_CELL((int)p0.x) - 1;
    int y0 = WORLD_TO_SPATIAL_CELL((int)p0.y) - 1;
    int x1 = WORLD_TO_SPATIAL_CELL((int)p1.x) + 1;
    int y1 = WORLD_TO_SPATIAL_CELL((int)p1.y) + 1;
    query_spatialmap(x0, y0, x1, y1, query_spatial_cb, &data);
}

uint32_t get_entity_render_key(int entityid)
{
    Entity *e = get_entity(entityid);

    uint32_t type = e->type;
    uint32_t texture = e->texture;
    AnimationFrame *frame = get_entity_animation_frame(e->id);

    if(frame)
        texture = frame->texture;

    uint32_t key = (type << 16) | (texture & 0xFFFF);

    return key;
}

void update_entity(int entityid)
{
    Entity *e = get_entity(entityid);

    EntityCallbacks *cb = getinsert_entity_callbacks(e->type);
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
            e->animation_frame_time += get_target_frame_interval() * e->animation_speed;
        }

        cb->update(e->id, cb->ctx);
    }
}

void entity_update_callback(uint32_t type, EntityUpdateFn callback)
{
    EntityCallbacks *cb = getinsert_entity_callbacks(type);
    cb->update = callback;
}

void entity_terminate_callback(uint32_t type, EntityTerminateFn callback)
{
    EntityCallbacks *cb = getinsert_entity_callbacks(type);
    cb->terminate = callback;
}

void entity_callback_context(uint32_t type, void *ctx)
{
    EntityCallbacks *cb = getinsert_entity_callbacks(type);
    cb->ctx = ctx;
}

void *get_entity_user_data(int entityid)
{
    Entity *entity = get_entity(entityid);
    uint8_t *addr = (uint8_t *)entity;

    addr += sizeof(Entity);

    return addr;
}

static EntityCallbacks *getinsert_entity_callbacks(uint32_t type)
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
    cb = mempool_take(&pool_callbacks);

    cb->type = type;
    cb->terminate = NULL;
    cb->update = NULL;
    cb->ctx = NULL;

    cb->next = root_callbacks;
    root_callbacks = cb;

    return cb;
}

AnimationFrame *get_entity_animation_frame(int entityid)
{
    Entity *e = get_entity(entityid);

    if(!e->animation)
        return NULL;

    if(e->animation->frame_count == 0)
        return NULL;

    if(e->animation->frame_count <= e->animation_frame)
        return NULL;

    return &e->animation->frames[e->animation_frame];
}

void entity_animation(int entityid, int animationid)
{
    Animation *animation = get_animation(animationid);
    Entity *e = get_entity(entityid);
    e->animation = animation;

    reset_entity_animation(entityid);
}

int get_entity_type(int entityid)
{
    Entity *e = get_entity(entityid);

    return e->type;
}

void entity_type(int entityid, int type)
{
    Entity *e = get_entity(entityid);
    e->type = type;
}

Vec3 get_entity_size(int entityid)
{
    Entity *e = get_entity(entityid);

    return e->size;
}

void entity_size(int entityid, Vec3 size)
{
    Entity *e = get_entity(entityid);
    e->size = size;
}

Collider *get_entity_collider(int entityid)
{
    Entity *e = get_entity(entityid);

    return &e->collider;
}

Vec3 get_entity_position(int entityid)
{
    Entity *e = get_entity(entityid);
    return e->position;
}

Vec3 get_entity_velocity(int entityid)
{
    Entity *e = get_entity(entityid);
    return e->velocity;
}

Vec4 get_entity_colormask(int entityid)
{
    Entity *e = get_entity(entityid);
    return e->colormask;
}

int get_entity_texture(int entityid)
{
    Entity *e = get_entity(entityid);
    return e->texture;
}

int get_entity_animation(int entityid)
{
    Entity *e = get_entity(entityid);

    if(!e->animation)
        return -1;

    return e->animation->id;
}

void entity_position(int entityid, Vec3 position)
{
    Entity *e = get_entity(entityid);
    e->position = position;
    int x = WORLD_TO_SPATIAL_CELL((int)position.x);
    int y = WORLD_TO_SPATIAL_CELL((int)position.y);
    move_spatialmap_node(&e->snode, x, y);
}

void entity_velocity(int entityid, Vec3 velocity)
{
    Entity *e = get_entity(entityid);
    e->velocity = velocity;
}

void entity_colormask(int entityid, Vec4 colormask)
{
    Entity *e = get_entity(entityid);
    e->colormask = colormask;
}

void entity_rotation(int entityid, Vec3 rotation)
{
    Entity *e = get_entity(entityid);
    e->rotation = rotation;
}

Vec3 get_entity_rotation(int entityid)
{
    Entity *e = get_entity(entityid);

    return e->rotation;
}
void entity_texture(int entityid, int texture)
{
    Entity *e = get_entity(entityid);
    e->texture = texture;
}

void entity_rotation_z(int entityid, float rotation)
{
    Entity *e = get_entity(entityid);
    e->rotation.z = rotation;
}

float get_entity_rotation_z(int entityid)
{
    Entity *e = get_entity(entityid);

    return e->rotation.z;
}

void reset_entity_animation(int entityid)
{
    Entity *e = get_entity(entityid);
    e->animation_frame = 0;
    e->animation_frame_time = 0.0f;
}

void entity_animation_speed(int entityid, float speed)
{
    Entity *e = get_entity(entityid);
    e->animation_speed = speed;
}
