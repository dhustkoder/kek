#include "kek.h"
#include <assert.h>

static MemPool pool;

void animation_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(Animation));
}

Animation *animation_create(void)
{
    Animation *animation = mem_pool_take(&pool);

    animation->frame_count = 0;
    animation->loop = 0;

    return animation;

}

void animation_destroy(Animation *animation)
{
    mem_pool_release(&pool, animation);
}

void animation_add_frame(Animation *animation, AnimationFrame frame)
{
    assert(animation->frame_count < MAX_ANIMATION_FRAMES);

    animation->frames[animation->frame_count] = frame;

    animation->frame_count++;
}

void animation_set_loop(Animation *animation, bool loop)
{
    animation->loop = loop;
}

