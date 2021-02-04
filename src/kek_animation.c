#include "kek.h"
#include <assert.h>

static struct mem_pool pool;

void animation_init(size_t capacity)
{
    mem_pool_alloc(&pool, capacity, sizeof(struct animation));
}

struct animation *animation_create(void)
{
    struct animation *animation = mem_pool_take(&pool);

    animation->frame_count = 0;
    animation->loop = 0;

    return animation;

}

void animation_destroy(struct animation *animation)
{
    mem_pool_release(&pool, animation);
}

void animation_add_frame(struct animation *animation, struct animation_frame frame)
{
    assert(animation->frame_count < MAX_ANIMATION_FRAMES);

    animation->frames[animation->frame_count] = frame;

    animation->frame_count++;
}

void animation_set_loop(struct animation *animation, bool loop)
{
    animation->loop = loop;
}
