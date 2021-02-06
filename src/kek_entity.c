#include "kek.h"

struct entity_callbacks {
    uint32_t type;
    KEKEntityInitFn init;
    KEKEntityTerminateFn terminate;
    KEKEntityUpdateFn update;
    void *ctx;
    struct entity_callbacks *next;
};

static struct mem_pool pool_entity;
static struct mem_pool pool_callbacks;
static struct entity_callbacks *root_callbacks = NULL;

static struct entity_callbacks *entity_getinsert_callbacks(uint32_t type);

void entity_init(size_t capacity, size_t type_capacity, size_t user_data_stride)
{
    mem_pool_alloc(&pool_entity, capacity, sizeof(struct entity) + user_data_stride);
    mem_pool_alloc(&pool_callbacks, type_capacity, sizeof(struct entity_callbacks));
    root_callbacks = NULL;
}

struct entity *entity_create(uint32_t type)
{
     struct entity *inst = mem_pool_take(&pool_entity);

    if(inst == NULL)
        return NULL;

    inst->destroy = false;
    inst->type = type;
    inst->scene_next_entity = NULL;
    inst->position = vec3_zero();
    inst->velocity = vec3_zero();
    inst->rotation = vec3_zero();
    inst->size     = vec3(128, 128, 128);

    inst->texture = NULL;
    inst->animation = NULL;
    inst->animation_frame = 0;
    inst->animation_speed = 1.0f;
    inst->animation_frame_time = 0.0f;

    struct entity_callbacks *cb = entity_getinsert_callbacks(type);
    if(cb && cb->init)
        cb->init(inst, cb->ctx);

    return inst;
}

void entity_destroy(struct entity *entity)
{
    mem_pool_release(&pool_entity, entity);
}

void entity_release(struct entity *entity)
{
    entity->destroy = true;
}

void entity_update(struct entity *e)
{
    struct entity_callbacks *cb = entity_getinsert_callbacks(e->type);
    if(cb && cb->update)
    {
        // update the animation
        if(e->animation && e->animation->frame_count > 0)
        {
            unsigned int idx = e->animation_frame;
            struct animation *anim = e->animation;
            struct animation_frame *frame = &anim->frames[idx];

            // safety to prevent bad timer and being in an infinite loop
            unsigned int counter = 0;
            while((e->animation_frame_time > frame->duration) && (counter < anim->frame_count))
            {
                e->animation_frame_time -= anim->frames[idx].duration * e->animation_speed;
                
                if(idx < anim->frame_count - 2)
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

void entity_set_init_callback(uint32_t type, KEKEntityInitFn callback)
{
    struct entity_callbacks *cb = entity_getinsert_callbacks(type);
    cb->init = callback;
}

void entity_set_update_callback(uint32_t type, KEKEntityUpdateFn callback)
{
    struct entity_callbacks *cb = entity_getinsert_callbacks(type);
    cb->update = callback;
}

void entity_set_terminate_callback(uint32_t type, KEKEntityTerminateFn callback)
{
    struct entity_callbacks *cb = entity_getinsert_callbacks(type);
    cb->terminate = callback;
}

void entity_set_callback_context(uint32_t type, void *ctx)
{
    struct entity_callbacks *cb = entity_getinsert_callbacks(type);
    cb->ctx = ctx;
}

void *entity_get_user_data(struct entity *entity)
{
    uint8_t *addr = (uint8_t *)entity;

    addr += sizeof(struct entity);

    return addr;
}

static struct entity_callbacks *entity_getinsert_callbacks(uint32_t type)
{
    struct entity_callbacks *cb = root_callbacks;

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
    cb->init = NULL;
    cb->terminate = NULL;
    cb->update = NULL;
    cb->ctx = NULL;

    cb->next = root_callbacks;
    root_callbacks = cb;

    return cb;
}

struct animation_frame *entity_get_animation_frame(struct entity *e)
{
    if(!e->animation)
        return NULL;

    if(e->animation->frame_count == 0)
        return NULL;

    if(e->animation->frame_count <= e->animation_frame)
        return NULL;

    return &e->animation->frames[e->animation_frame];
}

void entity_set_animation(struct entity *e, struct animation *animation)
{
    e->animation = animation;

    entity_reset_animation(e);
}

void entity_reset_animation(struct entity *e)
{
    e->animation_frame = 0;
    e->animation_frame_time = 0.0f;
}

void entity_set_animation_speed(struct entity *e, float speed)
{
    e->animation_speed = speed;
}
