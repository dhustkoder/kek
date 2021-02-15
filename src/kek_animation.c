#include "kek.h"
#include <assert.h>

static MemPool pool;

void init_animation(size_t capacity)
{
    mempool_alloc(&pool, capacity, sizeof(Animation));
}

Animation *create_animation(void)
{
    Animation *animation = mempool_take(&pool);

    animation->frame_count = 0;
    animation->loop = 0;

    return animation;

}

void destroy_animation(Animation *animation)
{
    mempool_release(&pool, animation);
}

void add_animation_frame(Animation *animation, AnimationFrame frame)
{
    assert(animation->frame_count < MAX_ANIMATION_FRAMES);

    animation->frames[animation->frame_count] = frame;

    animation->frame_count++;
}

void add_animation_frame_clip(Animation *animation, int textureid, int x, int y, int clip_width, int clip_height, float duration)
{
    AnimationFrame frame;
    Texture *texture = get_texture(textureid);

    // flip y
    y = texture->height - y - clip_height;
    
    frame.texture = textureid;
    frame.uv0.x = (float)x/(float)texture->width;
    frame.uv0.y = (float)y/(float)texture->height;
    frame.uv1.x = (float)(x + clip_width)/(float)texture->width;
    frame.uv1.y = (float)(y + clip_height)/(float)texture->height;
    frame.duration = duration;

    add_animation_frame(animation, frame);
}

void animation_loop(Animation *animation, bool loop)
{
    animation->loop = loop;
}

